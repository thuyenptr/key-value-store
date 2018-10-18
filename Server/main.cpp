#include "include/server.h"
#include <signal.h>
using namespace std;

void signalHandler(int signum) {
    cout << "Interrupt signal " << signum << endl;

    exit(signum);
}

int main() {
    signal(SIGINT, signalHandler);
    Server *server = new Server();
    server->Create();
    server->Listen();
    cout << "Server: Waiting for connections...\n";
    server->handleEventLoop();
    return 0;
}