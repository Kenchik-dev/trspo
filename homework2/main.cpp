#include <iostream>
#include <thread>
#include <queue>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <numeric>

constexpr int NUMBERS = 100000;
constexpr int THREAD_COUNT = 8;

std::mutex queue_mutex;
std::condition_variable queue_cv;
std::queue<int> task_queue;
bool done = false;

std::mutex result_mutex;
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

void worker() {
    while (true) {
        int number;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            queue_cv.wait(lock, [] { return !task_queue.empty() || done; });

            if (done && task_queue.empty())
                break;

            number = task_queue.front();
            task_queue.pop();
        }

        int steps = collatz_steps(number);

        {
            std::lock_guard<std::mutex> lock(result_mutex);
            results[number - 1] = steps;
        }
    }
}

int main() {
    results.resize(NUMBERS, 0);

    // Створення потоків
    std::vector<std::thread> threads;
    for (int i = 0; i < THREAD_COUNT; ++i) {
        threads.emplace_back(worker);
    }

    // Заповнення черги
    for (int i = 1; i <= NUMBERS; ++i) {
        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            task_queue.push(i);
        }
        queue_cv.notify_one();
    }

    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        done = true;
    }
    queue_cv.notify_all();

    for (auto &thread : threads) {
        thread.join();
    }

    for (int i = 1; i <= NUMBERS; ++i) {
        std::cout << "Number: " << i << ", Steps: " << results[i - 1] << std::endl;
    }

    long long total_steps = std::accumulate(results.begin(), results.end(), 0LL);
    double average_steps = static_cast<double>(total_steps) / NUMBERS;

    std::cout << "Average amount of steps: " << average_steps << std::endl;

    return 0;
}

