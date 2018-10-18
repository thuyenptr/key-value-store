//
// Created by lap11852 on 11/10/2018.
//
#pragma once

#include <pthread.h>
#include <unistd.h>
#include <deque>
#include <iostream>
#include <vector>
#include <errno.h>
#include <string.h>

#include "Global.h"


class Mutex {
public:
    Mutex();
    ~Mutex();
    void lock();
    void unlock();
    pthread_mutex_t* get_mutex_ptr();
    volatile bool is_locked;


private:
    pthread_mutex_t m_lock;
};

