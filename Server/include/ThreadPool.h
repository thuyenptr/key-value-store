//
// Created by lap11852 on 11/10/2018.
//

#pragma once

#include "Mutex.h"
#include "Task.h"
#include "CondVar.h"
#include "Global.h"


class ThreadPool {
private:
    int m_pool_size;
    Mutex m_task_mutex;
    CondVar m_task_cond_var;
    vector<pthread_t> m_threads;
    volatile int m_pool_state;
public:
    deque<Task*> m_tasks;

    ThreadPool();
    ThreadPool(int pool_size);
    ~ThreadPool();

    int init_threadpool();
    int destroy_threadpool();
    void* execute_thread();
    int add_task(Task* task);
};


