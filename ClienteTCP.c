/* client.c - code for example client program that uses TCP
cd /home/navenave/TrabalhoSO
gcc -o client ClienteTCP.c
./client
*/
#ifndef unix
#define WIN32
#include <windows.h>
#include <winsock.h>
#else
#define closesocket close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
struct timeval start, tv;

#define PROTOPORT       5193            /* default protocol port number */

extern  int             errno;
char    localhost[] =   "localhost";    /* default host name            */
/*------------------------------------------------------------------------
 * Program:   client
 *
 * Purpose:   allocate a socket, connect to a server, and print all output
 *
 * Syntax:    client [ host [port] ]
 *
 *               host  - name of a computer on which server is executing
 *               port  - protocol port number server is using
 *
 * Note:      Both arguments are optional.  If no host name is specified,
 *            the client uses "localhost"; if no protocol port is
 *            specified, the client uses the default given by PROTOPORT.
 *
 *------------------------------------------------------------------------
 */
void main(argc, argv)
int     argc;
char    *argv[];
{
        struct  hostent  *ptrh;  /* pointer to a host table entry       */
        struct  protoent *ptrp;  /* pointer to a protocol table entry   */
        struct  sockaddr_in sad; /* structure to hold an IP address     */
        int     sd;              /* socket descriptor                   */
        int     port;            /* protocol port number                */
        char    *host;           /* pointer to host name                */
        int     n;               /* number of characters read           */
        int     b=0, i=0;
        char    buf[1000];       /* buffer for data from the server     */
#ifdef WIN32
        WSADATA wsaData;
        WSAStartup(0x0101, &wsaData);
#endif
        memset((char *)&sad,0,sizeof(sad)); /* clear sockaddr structure */
        sad.sin_family = AF_INET;         /* set family to Internet     */

        /* Check command-line argument for protocol port and extract    */
        /* port number if one is specified.  Otherwise, use the default */
        /* port value given by constant PROTOPORT                       */

        if (argc > 2) {                 /* if protocol port specified   */
                port = atoi(argv[2]);   /* convert to binary            */
        } else {
                port = PROTOPORT;       /* use default port number      */
        }
        if (port > 0)                   /* test for legal value         */
                sad.sin_port = htons((u_short)port);
        else {                          /* print error message and exit */
                fprintf(stderr,"bad port number %s\n",argv[2]);
                exit(1);
        }

        /* Check host argument and assign host name. */

        if (argc > 1) {
                host = argv[1];         /* if host argument specified   */
        } else {
                host = localhost;
        }

        /* Convert host name to equivalent IP address and copy to sad. */

        ptrh = gethostbyname(host);
        if ( ((char *)ptrh) == NULL ) {
                fprintf(stderr,"invalid host: %s\n", host);
                exit(1);
        }
        memcpy(&sad.sin_addr, ptrh->h_addr, ptrh->h_length);

        /* Map TCP transport protocol name to protocol number. */

        if ( ((int*)(ptrp = getprotobyname("tcp"))) == 0) {
                fprintf(stderr, "cannot map \"tcp\" to protocol number");
                exit(1);
        }

        /* Create a socket. */

        sd = socket(PF_INET, SOCK_STREAM, ptrp->p_proto);
        if (sd < 0) {
                fprintf(stderr, "socket creation failed\n");
                exit(1);
        }

        /* Connect the socket to the specified server. */

        if (connect(sd, (struct sockaddr *)&sad, sizeof(sad)) < 0) {
                fprintf(stderr,"connect failed\n");
                exit(1);
        }
/* Definindo estrutura para utilizar comando select()           */

        gettimeofday(&start, NULL);
        fd_set reading;
        int retval;
        FD_ZERO(&reading);
        FD_SET(sd, &reading);

        tv.tv_sec = 1;
        tv.tv_usec = 0;

        /* Repeatedly read data from socket and write to user's screen. */
		//memset(buf,0, 1000);
        /*n = recv(sd, buf, sizeof(buf), 0);
        while (n > 0) {
                printf("%s",buf);
				memset(buf,0, 1000);
                n = recv(sd, buf, sizeof(buf), 0);
        }*/
        memset(buf,0,1024);
        b=recv(sd, buf, sizeof(buf), 0);
        buf[b]=0;
        printf("%s",buf);

        /* La??o for() feito para enviar m??ltiplas requisi????es. max_i = 1500 pois n??o tem garantia que todos os recv ser??o enviados*/

	sem_t NUM;
	sem_t FRASE;
	sem_init(&NUM, 0, 1);
		
       for(i=0;i<20;i++){  
	
            send(sd, "GETR\n",6, 0);
            
            sem_wait(&NUM);
            send(sd, "GETN\n",6, 0);
            
            send(sd, "10\n",4, 0);
            memset(buf,0,1024);
            b=recv(sd, buf, 1024, 0);
            buf[b]=0;
            printf("%s",buf);
        sem_post(&NUM);
            
            sem_wait(&NUM);
            send(sd, "REPL\n",6, 0);
            
            send(sd, "17\n",4, 0);

            send(sd, "SUBSTITUIDO\n",15, 0);
            memset(buf,0,1024);
            b=recv(sd, buf, 1024, 0);
            buf[b]=0;
            printf("%s",buf);
        sem_post(&NUM);

            send(sd, "PALT\n",6, 0);

sem_wait(&NUM);
            send(sd, "DELE\n",6, 0);
            
            send(sd, "15\n",4, 0);
           memset(buf,0,1024);
            b=recv(sd, buf, 1024, 0);
            buf[b]=0;
            printf("%s",buf);
        sem_post(&NUM);
            send(sd, "ADDF\n",6, 0);
            sem_wait(&NUM);

            send(sd, "NovaFrase\n",11, 0);
memset(buf,0,1024);
            b=recv(sd, buf, 1024, 0);
            buf[b]=0;
            printf("%s",buf);
        sem_post(&NUM);
        
            send(sd, "SEAR\n",6, 0);
        sem_wait(&NUM);    
                send(sd, "Pessoa\n",8, 0);
          memset(buf,0,1024);
            b=recv(sd, buf, 1024, 0);
            buf[b]=0;
            printf("%s",buf);
        sem_post(&NUM);
            
            send(sd, "PALD\n",6, 0);
          
               send(sd, "20\n",4, 0);
           memset(buf,0,1024);
            b=recv(sd, buf, 1024, 0);
            buf[b]=0;
            printf("%s",buf);
        sem_post(&NUM);
            send(sd, "VERS\n",6, 0);

            send(sd, "GRAV\n",6, 0);


            //send(sd, "CARR\n",6, 0);
            
            memset(buf,0,1024);
            b=recv(sd, buf, 1024, 0);
            buf[b]=0;
            printf("%s",buf);
        
        }
        send(sd, "CARR\n",6, 0);
            
            memset(buf,0,1024);
            b=recv(sd, buf, 1024, 0);
            buf[b]=0;
            printf("%s",buf);
        //send(sd, "QUIT\n",6, 0);
        /*Comando select() utilizado para estabelecer um timeout e garantir que o cliente n??o vai ficara parado aguardando todos os dados dos recv quando n??o forem enviados */

        n = select(sd +1, &reading, NULL, NULL, &tv);
        while (select(sd +1, &reading, NULL, NULL, &tv)!=0){
            memset(buf,0,1024);
            b=recv(sd, buf, 1024, 0);
            buf[b]=0;
            printf("%s",buf);
        }

        /* Close the socket. */

        close(sd);

        /* Terminate the client program gracefully. */

        exit(0);
}
