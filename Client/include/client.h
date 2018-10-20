//
// Created by lap11852 on 24/09/2018.
//

#ifndef CLIENT_CLIENT_H
#define CLIENT_CLIENT_H

#include "netdb.h"
#include <fcntl.h>
#include "stdlib.h"
#include "unistd.h"
#include <string.h>
#include <iostream>
#include "strings.h"
#include "sys/types.h"
#include <arpa/inet.h>
#include "sys/socket.h"
#include "netinet/in.h"
#include "../build/gen/Communication.pb.h"


#define CMD_GET "get"
#define CMD_SET "set"
#define CMD_EXIST "exist"
#define CMD_REMOVE "remove"
#define BILLION 1000000000L

using namespace std;

class Client {
private:
    int sockfd;
    int port;
    struct sockaddr_in serv_addr;
public:
    Client();
    ~Client();
    void Create();
    void setSocketOption(char* host, int port);
    void Connect();
    void eventLoop();
    void eventLoop(FILE*, string, string);
    bool ReceiveMessage();
    bool SendMessage(char*);
};


#endif //CLIENT_CLIENT_H
