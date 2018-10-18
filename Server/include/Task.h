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

using namespace std;


class Task {
public:
    Task(void (*func_ptr)(void*), void* arg);
    ~Task();
    void run();
private:
    void (*m_func_ptr)(void*);
    void* m_arg;
};

