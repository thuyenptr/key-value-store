//
// Created by lap11852 on 11/10/2018.
//

#include "../include/Mutex.h"

Mutex::Mutex() {
    pthread_mutex_init(&this->m_lock, nullptr);
    this->is_locked = false;
}

Mutex::~Mutex() {
    while(is_locked);
    unlock();
    pthread_mutex_destroy(&this->m_lock);
}

void Mutex::lock() {
    pthread_mutex_lock(&this->m_lock);
    this->is_locked = true;
}

void Mutex::unlock() {
    is_locked = false;
    pthread_mutex_unlock(&this->m_lock);
}

pthread_mutex_t* Mutex::get_mutex_ptr() {
    return &this->m_lock;
}