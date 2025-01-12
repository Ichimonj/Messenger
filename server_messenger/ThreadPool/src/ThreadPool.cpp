#include"ThreadPool.hpp"

ThreadPool::ThreadPool(size_t thread_size)
{
    threads.resize(thread_size);
    for (auto& th : threads) {
        th = std::thread([this]() { runThread(); });
        th.detach();
    }
}

ThreadPool::~ThreadPool()
{
    for (auto& th : threads) {
        th.~thread();
    }
}

template<typename F, typename ...Argc>
auto ThreadPool::addTask(F&& f, Argc && ...argc) -> std::future<decltype(f(argc ...))>
{
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

void ThreadPool::runThread()
{
    while (true) {
        std::function<void()> taskFunc;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            cv.wait(lock, [this] { return !task_queue.empty(); }); 
            taskFunc = task_queue.front();
            task_queue.pop();
        }
        taskFunc(); 
    }
}