#include"ThreadPool.hpp"
#include <iostream>
#include <string>
std::shared_ptr<ThreadPool> threadPool;

ThreadPool::ThreadPool(size_t thread_size)
{
    tpDEBUG_LOG("ThreadPool debug", "ThreadPool()");
    tpDEBUG_LOG("ThreadPool debug", (to_string(thread_size) + " threads"));

    threads.resize(thread_size);
    for (auto& th : threads) {
        th = std::thread([this]() { runThread(); });
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
        std::function<void()> taskFunc;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            cv.wait(lock, [this] { return !task_queue.empty(); }); 
            taskFunc = task_queue.front();
            task_queue.pop();
        }
        taskFunc(); 
        tpDEBUG_LOG("ThreadPool debug", "end work task");
    }
}