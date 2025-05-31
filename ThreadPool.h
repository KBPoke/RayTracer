#pragma once
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <future>

class ThreadPool {
private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    std::mutex queue_mutex;
    std::condition_variable queue_condition;
    std::atomic<bool> stopped;
public:
    ThreadPool(size_t thread_num) : stopped(false) {
        for (size_t i = 0; i < thread_num; i++) {
            workers.emplace_back([this] {
                while (true) {
                    std::function<void()> task;

                    {
                        std::unique_lock<std::mutex> queue_lock(this->queue_mutex);
                        queue_condition.wait(queue_lock, [this] {
                            return this->stopped || !this->tasks.empty();
                        });
                        if (this->stopped) {
                            return;
                        }

                        task = std::move(this->tasks.front());
                        tasks.pop();
                    }

                    task();
                }
            });
        }
    }

    ~ThreadPool() {
        stopped = true;
        queue_condition.notify_all();
        for (auto& worker : workers) {
            worker.join();
        }
    }

    std::future<void> add_task(std::function<void()>&& f) {
        auto task = std::make_shared<std::packaged_task<void()>>(std::move(f));
        std::future<void> result = task->get_future();

        {
            std::unique_lock<std::mutex> lock(queue_mutex);

            tasks.emplace([task] {(*task)(); });
        }
        queue_condition.notify_one();

        return result;
    }
};