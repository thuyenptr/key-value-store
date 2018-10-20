#include "include/client.h"
#include "include/config.h"

#include <pthread.h>
using namespace std;

struct m_param {
    string mode;
    string id;
    string type;
    FILE* file;
};

void* threadExecute(void *param);

void signalHandler(int signum);

void test_multi_client(FILE*, string type);

void test_single_client();

int main(int argc, char *argv[]) {
    signal(SIGINT, signalHandler);
    if (argc < 2) {
        cerr << "Miss argument" << endl;
        cerr << "-s: Single client test" << endl;
        cerr << "-m: Multi client test auto" << endl;
        cerr << "-m -g: Get command" << endl;
        cerr << "-m -s: Set command" << endl;
        cerr << "-m -r: Remove command" << endl;
        cerr << "-m -e: Exist command" << endl;

        exit(EXIT_FAILURE);
    }

    FILE* file = fopen("data.csv", "w+");

    if (string(argv[1]) == "-s") {
        test_single_client();
    } else if (string(argv[1]) == "-m") {
        test_multi_client(file, string(argv[2]));
    } else {
        cerr << "Error argument" << endl;
    }
    return 0;
}

void test_single_client() {
    pthread_t m_thread;
    m_param param;
    param.mode = "0";
    param.id = "";
    param.type = "";
    pthread_create(&m_thread, nullptr, &threadExecute, &param);
    pthread_join(m_thread, nullptr);
}

void test_multi_client(FILE* file, string type) {
    pthread_t threads[15];
    m_param pr[10];
    for (int i = 0; i < 10; ++i) {
        pr[i].mode = string("1");
        pr[i].id = toascii(i+65);
        pr[i].type = type;
        pr[i].file = file;
    }
    for (int i = 0; i < 10; ++i) {
        pthread_create(&threads[i], nullptr, &threadExecute, &pr[i]);
    }

    for (int i = 0; i < 10; ++i) {
        pthread_join(threads[i], nullptr);
    }
}

void* threadExecute(void *param) {
    m_param *p = (m_param*)(param);
    string mode = p->mode;
    string id = p->id;
    string type = p->type;

    FILE* file = p->file;


    Client* client = new Client();
    //client->Create();
    //client->setSocketOption(host_to_connect, PORT);
    //client->Connect();
    if (mode == "0") {
        client->eventLoop();
    } else if (mode == "1") {
        client->eventLoop(file, id, type);
    }
    //client->eventLoop();
}

void signalHandler(int signum) {
    cout << "Interrupt signal " << signum << endl;
    exit(signum);
}