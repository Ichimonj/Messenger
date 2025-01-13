#pragma once
#include <thread>
#include <queue>
#include <mutex>
#include <functional>
#include <vector>
#include <future>
#include "Preprocessor.hpp"

using namespace std;
class ThreadPool {
public:
    ThreadPool(size_t thread_size);
    ~ThreadPool();

public:
    template<typename F, typename... Argc>
    auto addTask(F&& f, Argc&&... argc) -> std::future<decltype(f(argc ...))>;

private:
    void runThread();

private:
    vector<thread> threads;
    queue<std::function<void()>> task_queue;
    mutex queue_mutex;
    std::condition_variable cv;
};

template<typename F, typename ...Argc>
auto ThreadPool::addTask(F&& f, Argc && ...argc) -> std::future<decltype(f(argc ...))>
{
    tpDEBUG_LOG("ThreadPool debug", "addTask");

    using return_type = decltype(f(argc...));
    auto taskPtr = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Argc>(argc)...)
    );
    std::future<return_type> result = taskPtr->get_future();
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        task_queue.push([taskPtr]() { (*taskPtr)(); });
        lock.unlock();
    }
    cv.notify_one();
    return result;
}
extern shared_ptr<ThreadPool> threadPool;