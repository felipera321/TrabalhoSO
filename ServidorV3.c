#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/select.h>
#include <sys/time.h>


#define PROTOPORT       5193            /* default protocol port number */
#define QLEN            10              /* size of request queue        */

int     visits      =   0;              /* counts client connections    */
char    msg [1700][1700];
int     ditados=0;
int     NumPalavras=0;
int     comandos=0;
char    aux[2000];

void LeDitado()
{
    FILE *arq;
    ditados=0;
    if ( (arq=fopen("Frases.txt","r")) == NULL ) { printf("\n Erro lendo arquivo ...\n\n");exit(0);}
    while (!feof(arq)) {

       fgets(msg[ditados],999,arq); printf("%d %s",ditados,msg[ditados]);
       ditados=(ditados+1)%1000;
    }


}

char uppercase(char *input) {
    char in=0;
    for(in = 0; (in<strlen(input)) && (in<1000); in++) {
        input[in] = toupper(input[in]);
    }
    return in;
}

void *atendeConexao( void *sd2 )
{
	int *temp=sd2;
    int sd=*temp;
	int sig=0;
	char str[2000], *endptr;
	int i=0, b=0, val, n;
    int req=0;


    visits++;

        while (1) {
		req++;
		sprintf(str,"\nRequisicao %d ", req);
		sig=send(sd,str,strlen(str),MSG_NOSIGNAL);
		if (sig==-1){
		break;
		}

		memset(str,0,999);
                b=recv(sd,str,999,0);
                for(n=0;n<b;n++){
                if((str[b-n] == '\n') || (str[b-n] == '\r')){
                str[b-n] = 0;
                }
                }
        comandos++;
		printf("\nComando recebido %d: %s",comandos, str);

		uppercase(str);
		//if((str[0]=='\r')||(str[0]=='\n')) continue;

		if (!strncmp(str,"GETR",4)) {
		     sprintf(str,"\nFrase: %s ", msg[random()%ditados]);
		     send(sd,str,strlen(str),0);
		}
                else
                if (!strncmp(str,"GETN",4)) {

                     b=recv(sd,str,1,0);
                     str[b]=0;
                     val = strtol(str, &endptr, 10); //MUDAR -1

                     //if (endptr==str)  {sprintf(str,"\nFALHA");continue;}
                     //else{
                     printf("\nGETN %d recebido",val);
                     sprintf(str,"\nGETN %d: %s",val, msg[val]);
                     send(sd,str,strlen(str),0);
                     //}
		}
                else
		if (!strncmp(str,"REPL",4)) {
		     b=recv(sd,str,2,0);
                     str[b]=0;
                     val = strtol(str, &endptr, 10);
                     //if (endptr==str)  {sprintf(str,"\nFALHA");continue;}
                     //else sprintf(str,"\nOK");
		     send(sd,str,strlen(str),0);
		     b=recv(sd,str,13,0);
             for(n=0;n<b;n++){              //LA?O FOR PARA LIMPAR AS ULTIMAS POSI??ES DO RECV, POIS MEU CLIENTE ENVIA \N JUNTO.
                if((str[b-n] == '\n') ||(str[b-n] == '\r')){
                    str[b-n] = 0;
                }       
            }
                     
                     strcpy(msg[val],str);
                     sprintf(str,"\nOK");
		     send(sd,str,strlen(str),0);
                     printf("\nNova frase %d: %s",val,msg[val]);
               }
               else
               if (!strncmp(str,"QUIT",	4)) {
		     sprintf(str,"\nAt?? Logo");
		     send(sd,str,strlen(str),0);
                     break;
               }
	       else
	       if (!strncmp(str,"VERS",4)) {
		     sprintf(str,"\nServidor de Frases 1.0 Beta.");
		     send(sd,str,strlen(str),0);
	       }
	      else
            if (!strncmp(str,"ADDF",4)) {
            // Add a new phrase
            b = recv(sd, str, 20, 0);
            str[b] = 0;
            strcpy(msg[ditados-1],str);
            char autor[]=". (Autor Desconhecido)";
            strcat(msg[ditados-1],autor);
            ditados++;
            sprintf(str, "\nPhrase added successfully.");
            send(sd, str, strlen(str), 0);
            //for(val=0;val<ditados;val++){
               // printf("%d %s",val,msg[val]);
            //}

}
            else
            if (!strncmp(str,"DELE",4)) {
            b = recv(sd, str, 10, 0);
            str[b] = 0;
            val = strtol(str, &endptr, 10);
            //if (endptr == str) {
            //sprintf(str, "\nFALHA");
            //continue;
            //}
            //else {
            strcpy(msg[val], "\0");
            sprintf(str, "\nOK\nFrase %d deletada\n",val);
            send(sd, str, strlen(str), 0);
            //}
}
           else
           if(!strncmp(str,"SEAR",4)){
            memset(str,0,1024);
            b=recv(sd,str,4,0);
            for(int n=0;n<b;n++){
            if((str[b-n]=='\n')||(str[b-n]=='\r')){
                str[b-n]=0;
            }
            }
            for(i=0;i<ditados;i++){
            if(strstr(msg[i],str)!=NULL){  //strstr compara se a palavra bate com a da frase
                send(sd,msg[i],strlen(msg[i]),0);
             }
            }
           }
           else
           if(!strncmp(str,"PALD",4)){
            b=recv(sd,str,1,0); //str -> teclado
            str[b]=0;
            val = strtol(str, &endptr, 10); //coleta o # da frase
            sprintf(str,"Ditado ecolhido %d: %s",val,msg[val]);
            send(sd,str,strlen(str),0);
            strcpy(aux,msg[val]);
            NumPalavras=0;
            for(i=0;aux[i]!='(';i++){
            if(aux[i] ==' ')
                NumPalavras++;
            }
            sprintf(str,"Numero de palavras do ditado %d: %d",val, NumPalavras);
            send(sd,str,strlen(str),0);
}
            else
            if(!strncmp(str,"PALT",4)){
            NumPalavras=0;
            for(val=0;val<ditados-1;val++){
            strcpy(aux,msg[val]);
            for(i=0;aux[i]!='(';i++){
            if(aux[i] ==' ')
                NumPalavras++;
            }
            }

            sprintf(str,"Numero de palavras TOTAL: %d", NumPalavras);
            send(sd,str,strlen(str),0);
            }

            else
            if(!strncmp(str,"GRAV",4)){
            FILE *f;
            if((f=fopen("ArquivoFrases.txt","w"))==NULL){
                printf("\nN?o foi poss?vel gravar\n");
                exit(0);
            }
            else{
            for(val=0;val<ditados;val++){
                fputs(msg[val],f);
            }}
            fclose(f);
            printf("\nArquivo gravado com sucesso.\n");
                sprintf(str,"\n Arquivo de frases 'ArquivoFrases.txt' gravado com sucesso.");
                send(sd,str,strlen(str),0);
            }
            else
            if(!strncmp(str,"CARR",4)){
            FILE *f;
            if((f=fopen("FrasesCARR.txt","r"))==NULL){
                printf("\nErro na Leitura");
                exit(0);
            }
            while(!feof(f)){
            fgets(msg[ditados],999,f);
            //printf("%d %s",ditados,msg[ditados]);
            ditados=(ditados+1)%1000;
            }

            }

	       else{
		     sprintf(str,"\nErro de Protocolo\n");
		     send(sd,str,strlen(str),0); //sd -> socket
	       }
       }
       close(sd);
}

int main(int argc, char *argv[])
{
	struct  protoent *ptrp;  /* pointer to a protocol table entry   */
	struct  sockaddr_in sad; /* structure to hold server's address  */
	struct  sockaddr_in cad; /* structure to hold client's address  */
    struct  timeval start, tv;
	int     sd=0 , sd2=0 ;         /* socket descriptors            */
    int     i;
	int     port;            /* protocol port number                */
	int     alen;            /* length of address                   */
    pthread_t t;
    srandom(time(NULL));
	memset((char *)&sad,0,sizeof(sad)); /* clear sockaddr structure */
	sad.sin_family = AF_INET;         /* set family to Internet     */
	sad.sin_addr.s_addr = INADDR_ANY; /* set the local IP address   */

	/* Check command-line argument for protocol port and extract    */
	/* port number if one is specified.  Otherwise, use the default */
	/* port value given by constant PROTOPORT                       */

	if (argc > 1) {                 /* if argument specified        */
		port = atoi(argv[1]);   /* convert argument to binary   */
	} else {
		port = PROTOPORT;       /* use default port number      */
	}
	if (port > 0)                   /* test for illegal value       */
		sad.sin_port = htons((u_short)port);
	else {                          /* print error message and exit */
		fprintf(stderr,"bad port number %s\n",argv[1]);
		exit(1);
	}

        LeDitado();

	/* Map TCP transport protocol name to protocol number */

	if ( ((ptrp = getprotobyname("tcp"))) == NULL) {
		fprintf(stderr, "cannot map \"tcp\" to protocol number");
		exit(1);
	}

	/* Create a socket */

	sd = socket(PF_INET, SOCK_STREAM, ptrp->p_proto);
	if (sd < 0) {
		fprintf(stderr, "socket creation failed\n");
		exit(1);
	}

	/* Bind a local address to the socket */

	if (bind(sd, (struct sockaddr *)&sad, sizeof(sad)) < 0) {
		fprintf(stderr,"bind failed\n");
		exit(1);
	}

	/* Specify size of request queue */

	if (listen(sd, QLEN) < 0) {
		fprintf(stderr,"listen failed\n");
		exit(1);
	}

	/* Main server loop - accept and handle requests */

	while (1) {
		alen = sizeof(cad);
		if ( (sd2=accept(sd, (struct sockaddr *)&cad, &alen)) < 0) {
			fprintf(stderr, "accept failed\n");
			exit(1);
		}
		printf ("\nServidor atendendo conex??o %d", visits);
                pthread_create(&t, NULL,  atendeConexao, &sd2 );

	}
}
