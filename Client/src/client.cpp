//
// Created by lap11852 on 24/09/2018.
//

#include "../include/client.h"

Client::Client() {

}

Client::~Client() {
    close(this->sockfd);
}

void Client::Create() {
    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->sockfd < 0) {
        perror("Create socket fail");
        exit(EXIT_FAILURE);
    }
}

void Client::setSocketOption(char* host, int port) {
    this->port = port;

    memset(&this->serv_addr, 0, sizeof(this->serv_addr));
    this->serv_addr.sin_family = AF_INET;
    this->serv_addr.sin_port = htons(static_cast<uint16_t>(this->port));

    if(inet_pton(AF_INET, host, &this->serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        exit(EXIT_FAILURE);
    }
}

void Client::Connect() {
    if (connect(this->sockfd, (struct sockaddr* ) &this->serv_addr, sizeof(this->serv_addr)) < 0) {
        perror("Connect");
        exit(EXIT_FAILURE);
    }

}

bool Client::ReceiveMessage() {
    char buffer[1024];
    int n = static_cast<int>(read(this->sockfd, buffer, sizeof(buffer)));
    if (n < 0) {
        return false;
    }
    buffer[n] = 0;
    Comm::Response response;
    response.ParseFromArray(buffer, n);
    if (response.sign() == "error") {
        cout << "Error" << endl;
        return false;
    } else if (response.sign() == "ok") {
        cout << "Message: " << response.message() << endl;
        cout << "Value: " << response.value() << endl;
        return true;
    } else {
        cout << "Can't receive data" << endl;
    }
}

bool Client::SendMessage(char* buffer) {
   if (send(this->sockfd, buffer, strlen(buffer), 0) < 0) {
       perror("Send");
       exit(EXIT_FAILURE);
   }
   return true;
}

void Client::eventLoop() {
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    string cmd;
    Comm::Request request;
    const string sign = "ok";
    timespec start, stop;
    double accum;
    while (true) {
        cout << "~ ";
        getline(cin, cmd);
        if (cmd == "exit") {
            close(this->sockfd);
            exit(EXIT_SUCCESS);
        } else {
            unsigned long pos = cmd.find_first_of('(');
            string type = cmd.substr(0, pos);
            string data = cmd.substr(pos + 1, cmd.length() - pos - 2);
            string key = data;
            string value;
            if (type == CMD_GET) {
                request.set_type(Comm::Type::GET);
            } else if (type == CMD_SET) {
                request.set_type(Comm::Type::SET);
                unsigned long comma_pos = data.find_first_of(',');
                key = data.substr(0, comma_pos);
                value = data.substr(comma_pos + 1, data.length() - comma_pos - 1);
            } else if (type == CMD_REMOVE) {
                request.set_type(Comm::Type::REMOVE);
            } else if (type == CMD_EXIST) {
                request.set_type(Comm::Type::EXIST);
            }

            request.set_key(key);
            request.set_value(value);
            request.set_sign(sign);
            char* pkg = new char[1024];
            request.SerializeToArray(pkg, 1024);

            clock_gettime(CLOCK_REALTIME, &start);
            this->SendMessage(pkg);
            this->ReceiveMessage();
            clock_gettime(CLOCK_REALTIME, &stop);

            accum = ((stop.tv_sec - start.tv_sec) + (stop.tv_nsec - start.tv_nsec)) / (BILLION * 1.0);
            cout << "Response time: " << accum << endl;
        }
    }
}

// void write_file(FILE* file, string key, string value) {

// }

void Client::eventLoop(string id, string type) {
    Comm::Request request;
    int i = 1, j = 1;
    //FILE* file = fopen("data.csv", "w+");
    const string sign = "ok";

    Comm::Type m_type;
    if (type == "-g") {
        m_type = Comm::Type::GET;
    } else if (type == "-r") {
        m_type = Comm::Type::REMOVE;
    } else if (type == "-s") {
        m_type = Comm::Type::SET;
    } else if (type == "-e") {
        m_type = Comm::Type::EXIST;
    }

    while (i < 5000) {
        string key = to_string(i);
        key = key + id;
        string value = to_string(j);

        cout << "Key: " << key << endl;
        cout << "value: " << value << endl;
        //write_file(file, key, value);

        request.set_type(m_type);
        request.set_key(key);
        request.set_value(value);
        request.set_sign(sign);

        char* pkg = new char[1024];
        int siz = request.ByteSize();
        request.SerializeToArray(pkg, siz);

        this->SendMessage(pkg);
        this->ReceiveMessage();

        ++i;
        ++j;
        delete[] pkg;
    }
}
