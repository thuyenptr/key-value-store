//
// Created by lap11852 on 24/09/2018.
//

#ifndef KEYVALUESTORE_SERVER_H
#define KEYVALUESTORE_SERVER_H
#include "../build/gen/Communication.pb.h"
#include "BTreeStorage.h"
#include "ThreadPool.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <iostream>
#include <string.h>
#include <ctime>
#include <fcntl.h>

#define  PORT 98080
#define BACKLOG 20 // number of connections allowed on the incoming queue

using namespace std;
using Comm::Response;
using Comm::Request;

struct task_data {
    char key[1024];
    char value[1024];
    int sock_id;
};

class Server {
private:
    int listener, newfd, fdmax;
    fd_set master;
    fd_set read_fds;
    struct sockaddr_in serv_addr, cli_addr;
    ThreadPool* pool;
    static KeyValueStore* store;
    static Mutex m_rc_mutex;
    static Mutex m_db_mutex;
    static volatile int rc;
public:
    Server();
    ~Server();
    void Create();
    void Listen();
    void handleEventLoop();
    bool setNonblocking(int&);
    bool handlePackageArrive(Request, int);
    void handleClientMessage(int);

    static void get_func(void* arg);
    static void set_func(void* arg);
    static void remove_func(void* arg);
    static void exist_func(void* arg);
};


#endif //KEYVALUESTORE_SERVER_H
