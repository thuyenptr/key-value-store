//
// Created by lap11852 on 11/10/2018.
//

#include "../include/ThreadPool.h"

ThreadPool::ThreadPool() : m_pool_size(DEFAULT_POOL_SIZE) {}

ThreadPool::ThreadPool(int pool_size) : m_pool_size(pool_size) {}

ThreadPool::~ThreadPool() {
    if (this->m_pool_state != STOPPED) {
        destroy_threadpool();
    }
}

void* start_thread(void* arg) {
    ThreadPool* threadPool = (ThreadPool*) arg;
    threadPool->execute_thread();
    return nullptr;
}

int ThreadPool::init_threadpool() {
    this->m_pool_state = STARTED;
    int ret;
    for (int i = 0; i < this->m_pool_size; ++i) {
        pthread_t thread_id;
        ret = pthread_create(&thread_id, nullptr, start_thread, (void*)this);
        if (ret != 0) {
            perror("pthread_create failed");
        }
        cout << "Thread " << i + 1 << " started." << endl;
        this->m_threads.push_back(thread_id);
    }
    return 0;
}

int ThreadPool::destroy_threadpool() {
    this->m_task_mutex.lock();
    this->m_pool_state = STOPPED;
    this->m_task_mutex.unlock();

    this->m_task_cond_var.broadcast();

    for (int i = 0; i < this->m_pool_size; ++i) {
        void* result;
        pthread_join(this->m_threads[i], &result);

        this->m_task_cond_var.broadcast();
    }
    return 0;
}

void* ThreadPool::execute_thread() {
    Task* task = nullptr;

    while (true) {
        // lock de pick task

        this->m_task_mutex.lock();

        while ((this->m_pool_state != STOPPED) && (this->m_tasks.empty())) {
            this->m_task_cond_var.wait(this->m_task_mutex.get_mutex_ptr());
        }

        if (this->m_pool_state == STOPPED) {
            this->m_task_mutex.unlock();
            pthread_exit(nullptr);
        }

        task = this->m_tasks.front();
        this->m_tasks.pop_front();
        this->m_task_mutex.unlock();

        (*task).run();

        delete task;
    }
}

int ThreadPool::add_task(Task *task) {
    this->m_task_mutex.lock();
    this->m_tasks.push_back(task);
    this->m_task_mutex.unlock();
    this->m_task_cond_var.signal();
    return 0;
}