#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <thread>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdexcept>
#include "cmdExec.h"

using namespace std;

void handle_client(int newConnSock)
{
    // inital send data
    char *connMsg = (char *)"Server Connected to Client!\n";
    char buffer[MAX_BUFF_SIZE];
    int rval;
    bool startUp = true; // initial connection bool

    // if client connects, send confirmation message, listen for data
    if (send(newConnSock, connMsg, strlen(connMsg), 0) < 0)
    {
        perror("Send failed");
        exit(1);
    }

    // loop through receiving data from client
    while (true)
    {

        // reset server incoming memory to buffer
        memset(&buffer, 0, sizeof(buffer));
        // receive and error check
        if ((rval = recv(newConnSock, &buffer, sizeof(buffer), 0)) < 0)
            perror("Reading stream message error");
        else if (rval == 0)
        {
            cout << "Ending connection... \n";
            break;
        }
        else
        {
            cout << "New Message from client: " << buffer << "\n";
        }

        // if input is 'exit', send client close info and break out of loop
        if (strcmp(buffer, "exit") == 0)
        {
            send(newConnSock, buffer, sizeof(buffer), 0);
            break;
        }

        // on first run, don't attempt to exec anything
        if (startUp)
        {
            startUp = false;
            continue;
        }

        // call the forking, exec, and piping function
        string output = "";
        try
        {
            output = cmdOutput(buffer);
        }
        catch (...)
        {
            output = "Not Found\n";
            perror("No output found");
        }

        // send the output of the cmdOutput call and error check
        if ((rval = send(newConnSock, (const void *)output.c_str(), output.length(), 0)) < 0)
            perror("cmd output send failed");
        else
        {
            cout << "Response Sent to Client!\n\n";
        }
    }

    // close client socket
    close(newConnSock);

    // server keeps running and listening to new client connection
    printf("Socket closed, listening for more connections... \n");
}

int main(int argc, char *argv[])
{

    // declare sockets and server info
    int serverSock;
    int newConnSock;
    sockaddr_in server, client_address;
    socklen_t client_address_len = sizeof(client_address);
    vector<thread> threads;

    // total number of connections allowed
    const int CONNECTIONS = 5;

    // initalize server socket
    serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock < 0)
    {
        perror("Socket creation failed");
        exit(1);
    }

    // intialize server information, set to any form of internet / localhost and port
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // bind the server socket to localhost server (and also our port 5001)
    if (bind(serverSock, (sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("Bind Failed");
        exit(1);
    }

    // listen on the bound socket, with up to 5 connections...
    if(listen(serverSock, CONNECTIONS) < 0)
    {
        perror("Connection not allowed");
        exit(1);
    }
    cout << "Server Booted Up!\n";
    cout << "Listening for new connections...\n\n";

    while (true)
    {
        newConnSock = accept(serverSock, (sockaddr *)&client_address, &client_address_len);
        if (newConnSock < 0)
        {
            perror("Accept failed");
        }
        threads.emplace_back(handle_client, newConnSock);
    }

    close(serverSock);
    return 0;
}
