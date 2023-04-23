#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <iostream>
#include "cmdExec.h"

using namespace std;

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        cerr << "Usage: ./client <host ip address> \n";
        return 1;
    }

    // declare send data, socket, server info, localhost, and buffer
    char *data = (char *)"Client Connected!";
    int clientSock;
    sockaddr_in server;
    hostent *hp;
    char buffer[MAX_BUFF_SIZE];
    int rval;

    // initialize socket, check for error
    clientSock = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSock < 0)
    {
        perror("socket failed");
        exit(1);
    }
    server.sin_family = AF_INET;

    // argv[1] for variable address
    hp = gethostbyname(argv[1]);

    if (hp == 0)
    {
        perror("Client connection failed");
        close(clientSock);
        exit(1);
    }

    // set memory to copy of the server initial state
    memcpy(&server.sin_addr, hp->h_addr, hp->h_length);
    server.sin_port = htons(PORT);

    // connect and error check
    if (connect(clientSock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("connect failed");
        close(clientSock);
        // exit(1);
    }

    // set an input string, init to welcome message
    string line = (string)data;
    while (clientSock >= 0)
    {
        
        // send data over to server, convert input 'line' to string
        if (send(clientSock, line.c_str(), strlen(line.c_str()), 0) < 0)
        {
            perror("send failed");
            close(clientSock);
            exit(1);
        }
    
        // exit loop and close socket with input == 'exit'
        if (strcmp(line.c_str(), "exit") == 0)
        {
            cout << "Connection cancelled!\n";
            send(clientSock, line.c_str(), sizeof(line.c_str()), 0);
            break;
        }
        
        rval = recv(clientSock, &buffer, sizeof(buffer), 0);
        
        // receive and error check
        if (rval < 0){
            perror("Reading stream message error");
        }
        else if (rval == 0)
        {
            cout << "No Response\n";
            cout << "Message complete\n";
        }
        else{
            cout << "New Message from Server: \n\n";
            cout << buffer;
            
        }
        cout << "\nMessage complete\n";

        cout << "Enter command: ";
        // interactive client input (continuously input and send data)
        getline(cin, line);

        // reset buffer
        memset(&buffer, 0, sizeof(buffer));
                

        // store line into buffer
        strcpy(buffer, line.c_str());
                

    }
    if(clientSock < 0)
    {
        cout<<"Error executing command";
    }

    // terminate client connection and exit client process
    close(clientSock);

    return 0;
}
