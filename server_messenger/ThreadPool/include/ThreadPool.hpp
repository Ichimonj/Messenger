#pragma once
#include <thread>
#include <queue>
#include <mutex>
#include <functional>
#include <vector>
#include <future>

using namespace std;
class ThreadPool {
public:
    ThreadPool(size_t thread_size);
    ~ThreadPool();

public:
    template<typename F, typename... Argc>
    auto addTask(F&& f, Argc&&... argc) -> std::future<decltype(f(argc...))>;

private:
    void runThread();

private:
    vector<thread> threads;
    queue<std::function<void()>> task_queue;
    mutex queue_mutex;
    std::condition_variable cv;
};
