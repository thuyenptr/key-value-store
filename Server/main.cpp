//#include "include/server.h"
#include "include/BTreeStorage.h"
#include <signal.h>
#include <vector>
using namespace std;

void signalHandler(int signum) {
    cout << "Interrupt signal " << signum << endl;

    exit(signum);
}

double cal_time(timespec start, timespec end) {
    return ((end.tv_sec-start.tv_sec) + (((end.tv_nsec - start.tv_nsec) * 1.0) / BILLION));
}

vector<DataStore> get_data() {
    FILE* file = fopen("data.csv", "r");
    vector<DataStore> records;
    char key[1024];
    char value[1024];
    while (fscanf(file, "%s %s", key, value) != EOF) {
        DataStore record;
        strcpy(record.key, key);
        strcpy(record.value, value);
        records.push_back(record);
    }
    fclose(file);
    return records;
}

void test_insert() {
    vector<DataStore> records = get_data();
    KeyValueStore* store = new BTreeStorage(const_cast<char *>("nodes.dat"), 0);

    timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < records.size() - 100; ++i) {
        store->set(records[i].key, records[i].value);
    }
    store->write_tree();
    clock_gettime(CLOCK_REALTIME, &end);

    cout << cal_time(start, end);
}


void run_test(string arg) {
    if (arg == "-b") {
        test_insert();
    }
}


int main(int argc, char* argv[]) {
    char* buf;
    signal(SIGINT, signalHandler);
//    Server *server = new Server();
//    server->Create();
//    server->Listen();
//    cout << "Server: Waiting for connections...\n";
//    server->handleEventLoop();
    if (argc < 2) {
        cerr << "Missing argument";
        exit(EXIT_FAILURE);
    }

    run_test(argv[1]);

    return 0;
}


