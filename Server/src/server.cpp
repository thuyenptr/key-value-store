//
// Created by lap11852 on 24/09/2018.
//

#include "../include/server.h"

KeyValueStore* Server::store = new BTreeStorage(const_cast<char *>("nodes.dat"), 1);
Mutex Server::m_db_mutex;
Mutex Server::m_rc_mutex;
volatile int Server::rc = 0;

Server::Server() {
    this->pool = new ThreadPool(DEFAULT_POOL_SIZE);
    this->pool->init_threadpool();
}

Server::~Server() {
    close(this->listener);
    close(this->newfd);
    delete store;
}

void Server::get_func(void *arg) {
    m_rc_mutex.lock(); // doc quyen truy xuat bien rc
    rc++;
    if (rc == 1) { // tien trinh doc dau tien => khong cho ghi
        m_db_mutex.lock();
    }
    m_rc_mutex.unlock();

    // read database
    Response res;
    char* pkg = new char[256];
    task_data* data = (task_data*)arg;

    if (string(data->key).empty()) {
        res.set_sign("error");
        res.set_message("Key error");
    } else {

        cout << "Get(" << data->key << ")" << endl;
        VALUE result = store->get(data->key);

        if (string(result).empty()) {
            res.set_sign("error");
            res.set_message("Key does not exist");
        } else {
            res.set_sign("ok");
            res.set_message("Get success");
            res.set_value(result);
        }
    }

    res.SerializeToArray(pkg, 256);

    write(data->sock_id, pkg, strlen(pkg));

    delete[] pkg;
    delete data;
    // end read database
    m_rc_mutex.lock(); // doc quyen truy xuat
    rc--; // giam tien trinh doc
    if (rc == 0) { // tien trinh doc cuoi cung
        m_db_mutex.unlock();
    }
    m_rc_mutex.unlock();


}

void Server::set_func(void *arg) {
    m_db_mutex.lock();
    char* pkg = new char[256];
    Response res;
    task_data* data = (task_data*)arg;
    if (string(data->key).empty()) {
        res.set_sign("error");
        res.set_message("Key error");
    } else {
        cout << "Set(" << data->key << "," << data->value << ")" << endl;
        store->set(data->key, data->value);
        store->write_tree();
        res.set_sign("ok");
        res.set_message("Set success");
    }

    res.SerializeToArray(pkg, 256);
    write(data->sock_id, pkg, strlen(pkg));

    delete[] pkg;
    delete data;
    m_db_mutex.unlock();
}

void Server::exist_func(void *arg) {
    m_rc_mutex.lock(); // doc quyen truy xuat rc
    rc++;
    if (rc == 1) { // tien trinh doc dau tien
        m_db_mutex.lock();
    }
    m_rc_mutex.unlock();
    // Tien hanh doc
    Response res;
    char* pkg = new char[256];
    task_data* data = (task_data*)arg;

    if (string(data->key).empty()) {
        res.set_sign("error");
        res.set_message("Key error");
    } else {

        cout << "Exist(" << data->key << ")" << endl;

        VALUE result = store->get(data->key);

        if (string(result).empty()) {
            res.set_sign("ok");
            res.set_message("Key does not exist");
        } else {
            res.set_sign("ok");
            res.set_message("Key already existed");
            res.set_value(result);
        }
    }

    res.SerializeToArray(pkg, 256);
    write(data->sock_id, pkg, strlen(pkg));
    delete[] pkg;
    delete data;
    // Hoan tat doc
    m_rc_mutex.lock();
    rc--;
    if (rc == 0) { // tien trinh doc cuoi cung
        m_db_mutex.unlock();
    }
    m_rc_mutex.unlock();
}

void Server::remove_func(void *arg) {
    m_db_mutex.lock();
    char* pkg = new char[256];
    Response res;
    task_data* data = (task_data*)arg;
    if (string(data->key).empty()) {
        res.set_sign("error");
        res.set_message("Key error");
    } else {
        cout << "Remove(" << data->key << ")" << endl;

        bool rs = store->remove(data->key);
        store->write_tree();
        if (rs) {
            res.set_message("Remove success");
            res.set_sign("ok");
        } else {
            res.set_sign("ok");
            res.set_message("Remove fail");
        }
    }

    res.SerializeToArray(pkg, 256);
    write(data->sock_id, pkg, strlen(pkg));
    delete[] pkg;
    delete data;
    m_db_mutex.unlock();
}

bool Server::setNonblocking(int& fd) {
    if (fcntl(fd, F_SETFL, 0) < 0) {
        return false;
    }
    return fcntl(fd, F_SETFL, O_NONBLOCK) >= 0;
}


void Server::Create() {
    // protocol family ipv4, protocal type: TCP, 0: default protocol used
    int yes = 1;

    this->listener = socket(AF_INET, SOCK_STREAM, 0);
    if (this->listener < 0) {
        perror("Create socket fail.");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(this->listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        perror("server: socket");
        exit(EXIT_FAILURE);
    }



    this->serv_addr.sin_family = AF_INET;
    this->serv_addr.sin_addr.s_addr = INADDR_ANY;
    this->serv_addr.sin_port = htons(static_cast<uint16_t>(PORT)); // host to network short, relate big-endian, little-endian



    int check = bind(this->listener, (struct sockaddr *) &this->serv_addr,
                     sizeof(serv_addr));
    if (check < 0) {
        perror("ERROR on binding");
        exit(EXIT_FAILURE);
    }
}

void Server::Listen() {
    if (listen(this->listener, BACKLOG) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    FD_SET(this->listener, &this->master); // add listener to the master set
    this->fdmax = this->listener;
}

void Server::handleEventLoop() {
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    int result;
    timeval tv;

    while (true) {
        memcpy(&this->read_fds, &this->master, sizeof(this->read_fds));
        tv.tv_sec = 30;
        tv.tv_usec = 0;
        this->read_fds = this->master; // copy
        result = select(this->fdmax + 1, &this->read_fds, nullptr, nullptr, &tv);
        if (result == 0) {
            cout << "select() timed out" << endl;
        } else if (result < 0 && errno != EINTR) {
            cout << "Error in select(): " << strerror(errno) << endl;
        } else if (result > 0) {
            if (FD_ISSET(this->listener, &this->read_fds)) {
                socklen_t socklen = sizeof(this->cli_addr);
                this->newfd = accept(this->listener,
                                     (struct sockaddr*)&this->cli_addr,
                                     &socklen);
                if (!this->setNonblocking(this->newfd)) {
                    perror("Set Non-Blocking");
                }

                if (this->newfd < 0) {
                    cout << "Error in accept(): " << strerror(errno) << endl;
                } else {
                    FD_SET(this->newfd, &master); // add to master
                    this->fdmax = (this->fdmax < this->newfd) ? this->newfd : this->fdmax;
                    cout << "Client: " << this->cli_addr.sin_addr.s_addr << " Port: " << this->cli_addr.sin_port << endl;
                }
                FD_CLR(this->listener, &this->read_fds);
            }
            for (int i = 0; i <= this->fdmax; ++i) {
                if (FD_ISSET(i, &this->read_fds)) {
                    this->handleClientMessage(i);
                }
            }
        }
    }
}

void Server::handleClientMessage(int i) {
    int nbytes;
    char buf[256];

    nbytes = static_cast<int>(recv(i, buf, sizeof(buf), 0));
    if (nbytes <= 0) {
        if (nbytes == 0) {
            //connection closed
            cout << "Client: " << i << " disconnected." << endl;
        } else {
            perror("read");
        }
        // close socket
        close(i);
        FD_CLR(i, &master);
    } else {
        if (FD_ISSET(i, &this->master)) {
            if (i != this->listener) {
                buf[nbytes] = 0;
                Request request;
                request.ParseFromArray(buf, sizeof(buf));

                this->handlePackageArrive(request, i);
            }
        }
    }
}


bool Server::handlePackageArrive(Request request, int sock_id) {
    if (request.sign() != "ok") {
        return false;
    } else {
        task_data* data = new task_data();
        strcpy(data->key,(char*)request.key().c_str());
        strcpy(data->value, (char*)request.value().c_str());
        data->sock_id = sock_id;
        int type = request.type();
        if (type == Comm::GET) {
            Task* task = new Task(&this->get_func, data);
            this->pool->add_task(task);
        } else if (type == Comm::SET) {
            Task* task = new Task(&this->set_func, data);
            this->pool->add_task(task);
        } else if (type == Comm::REMOVE) {
            Task* task = new Task(&this->remove_func, data);
            this->pool->add_task(task);
        } else if (type == Comm::EXIST) {
            Task* task = new Task(&this->exist_func, data);
            this->pool->add_task(task);
        }
    }
    return true;
}