#pragma once
#include <thread>
#include <queue>
#include <mutex>
#include <functional>
#include <vector>
#include <future>
#include "preprocessor.hpp"

class ThreadPool {
public:
    ThreadPool(size_t thread_size);
    ~ThreadPool();

public:
    template<typename F, typename... Argc>
    auto addTask(F&& f, Argc&&... argc) -> future<decltype(f(argc ...))>;

private:
    void runThread();

private:
    vector<thread>          threads;
    queue<function<void()>> task_queue;
    mutex                   cv_mutex;
    mutex                   queue_mutex;
    condition_variable      cv;
};

template<typename F, typename ...Argc>
auto ThreadPool::addTask(F&& f, Argc && ...argc) -> future<decltype(f(argc ...))>
{
    tpDEBUG_LOG("ThreadPool debug", "addTask");

    using return_type = decltype(f(argc...));
    auto taskPtr = make_shared<packaged_task<return_type()>>(
        bind(forward<F>(f), forward<Argc>(argc)...)
    );
    future<return_type> result = taskPtr->get_future();
    {
        queue_mutex.lock();
        task_queue.push([taskPtr]() { (*taskPtr)(); });
        queue_mutex.unlock();
    }
    cv.notify_one();
    return result;
}
extern shared_ptr<ThreadPool> threadPool;