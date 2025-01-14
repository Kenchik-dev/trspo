#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <numeric>

constexpr int NUMBERS = 100000;
constexpr int THREAD_COUNT = 8;

std::vector<int> results;

// Функція для обчислення кількості кроків відповідно до гіпотези Колаца
int collatz_steps(int n) {
    int steps = 0;
    while (n != 1) {
        if (n % 2 == 0)
            n /= 2;
        else
            n = 3 * n + 1;
        ++steps;
    }
    return steps;
}

class ThreadPool {
private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;

public:
    ThreadPool(size_t threads) : stop(false) {
        for (size_t i = 0; i < threads; ++i) {
            workers.emplace_back([this] {
                for (;;) {
                    std::function<void()> task;

                    {
                        std::unique_lock<std::mutex> lock(this->queue_mutex);
                        this->condition.wait(lock, [this] { return this->stop || !this->tasks.empty(); });
                        if (this->stop && this->tasks.empty())
                            return;
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }

                    task();
                }
            });
        }
    }

    template <class F>
    void enqueue(F&& f) {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            if (stop)
                throw std::runtime_error("enqueue on stopped ThreadPool");
            tasks.emplace(std::forward<F>(f));
        }
        condition.notify_one();
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            stop = true;
        }
        condition.notify_all();
        for (std::thread &worker : workers)
            worker.join();
    }
};

int main() {
    // Ініціалізація результатів
    results.resize(NUMBERS, 0);

    // Створення пулу потоків
    ThreadPool pool(THREAD_COUNT);

    for (int i = 1; i <= NUMBERS; ++i) {
        pool.enqueue([i] {
            int steps = collatz_steps(i);
            results[i - 1] = steps;
        });
    }

    // Виведення кількості кроків для кожного числа
    for (int i = 1; i <= NUMBERS; ++i) {
        std::cout << "Number: " << i << ", Steps: " << results[i - 1] << std::endl;
    }

    // Підрахунок середнього значення
    long long total_steps = std::accumulate(results.begin(), results.end(), 0LL);
    double average_steps = static_cast<double>(total_steps) / NUMBERS;

    std::cout << "Average number of steps: " << average_steps << std::endl;

    return 0;
}