//
// Created by lap11852 on 11/10/2018.
//

#include "../include/Task.h"
#include <iostream>
using namespace std;

Task::Task(void (*func_ptr)(void*), void *arg) {
    this->m_func_ptr = func_ptr;
    this->m_arg = arg;
}

Task::~Task() {
}

void Task::run() {
    (*this->m_func_ptr)(this->m_arg);
}