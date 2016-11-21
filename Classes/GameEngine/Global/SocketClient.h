//
// Created by igor on 21.11.16.
//

#ifndef ARCHERSUNLIMITED_SOCKETCLIENT_H
#define ARCHERSUNLIMITED_SOCKETCLIENT_H

#include "network/SocketIO.h"

#include<iostream>    //cout
#include<stdio.h> //printf
#include<string.h>    //strlen
#include<string>  //string
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include<netdb.h> //hostent

using namespace std;

/**
    TCP Client class
*/
class SocketClient
{
private:
    int sock;
    std::string address;
    int port;
    struct sockaddr_in server;

public:
    SocketClient();
    bool conn(string, int);
    bool send_data(string data);
    string receive(int);
};


#endif //ARCHERSUNLIMITED_SOCKETCLIENT_H
