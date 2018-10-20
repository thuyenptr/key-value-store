//
// Created by lap11852 on 11/10/2018.
//

#include "../include/CondVar.h"

CondVar::CondVar() {
    pthread_cond_init(&this->m_cond_var, NULL);
}

CondVar::~CondVar() {
    pthread_cond_destroy(&this->m_cond_var);
}

void CondVar::wait(pthread_mutex_t *mutex) {
    pthread_cond_wait(&this->m_cond_var, mutex);
}

void CondVar::signal() {
    pthread_cond_signal(&this->m_cond_var);
}

void CondVar::broadcast() {
    pthread_cond_broadcast(&this->m_cond_var);
}