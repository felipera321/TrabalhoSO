#!/bin/bash
gcc -o servidorF servidorF.c -lpthread
gcc -o client client.c

gnome-terminal -x sh -c "./servidorF; bash"
for (( i=1; i<=10; i++ ))
do  
   echo "Cliente Aberto" $i
   gnome-terminal -x sh -c "./client; bash"
done

