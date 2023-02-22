#!/bin/bash

# Compile the server program
gcc -o servidorF servidorF.c -lpthread

# Run the server in the background
./servidorF &

# Wait for the server to start up
sleep 1

# Compile the client program
gcc -o client client.c

# Loop 10 times to run 10 clients
for i in {1..10}
do
    # Start the client in the background
    ./client &
done

# Wait for all clients to finish
wait

# Kill the server process
pkill servidorF
