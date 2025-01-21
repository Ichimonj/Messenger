#include"ThreadPool.hpp"
#include <iostream>
#include <string>
shared_ptr<ThreadPool> threadPool;

ThreadPool::ThreadPool(size_t thread_size)
{
    tpDEBUG_LOG("ThreadPool debug", "ThreadPool()");
    tpDEBUG_LOG("ThreadPool debug", (to_string(thread_size) + " threads"));

    threads.resize(thread_size);
    for (auto& th : threads) {
        th = thread([this]() { runThread(); });
        th.detach();
    }
}

ThreadPool::~ThreadPool()
{
    tpDEBUG_LOG("ThreadPool debug", "~ThreadPool()");
    for (auto& th : threads) {
        th.~thread();
    }
}

void ThreadPool::runThread()
{
    tpDEBUG_LOG("ThreadPool debug", "runThread()");
    while (true) {
        function<void()> taskFunc;
        {
            unique_lock<mutex> lock(cv_mutex);
            cv.wait(lock, [this] { return !task_queue.empty(); }); 
            queue_mutex.lock();
            taskFunc = task_queue.front();
            task_queue.pop();
        }
        taskFunc(); 
        queue_mutex.unlock();
        tpDEBUG_LOG("ThreadPool debug", "end work task");
    }
}