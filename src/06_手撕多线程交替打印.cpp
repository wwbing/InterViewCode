#include <iostream>
#include <chrono>
#include <cstdio>
#include <mutex>
#include <thread>
#include <vector>
#include <atomic>
#include <condition_variable>

// 手撕多线程交替打印实现
std::mutex mux;
std::condition_variable cond;
std::atomic<int> current_id{0};
const int NUM_THREADS = 3;
std::atomic<bool> print_running{true};

void multi_thread_print(int thread_id)
{
    int print_count = 0;

    while (print_running.load() && print_count < 10) {
        std::unique_lock<std::mutex> lock(mux);

        // 等待轮到自己或停止运行
        cond.wait(lock, [thread_id]() {
            return thread_id == current_id.load() || !print_running.load();
        });

        // 如果停止运行，退出
        if (!print_running.load()) {
            break;
        }

        // 再次判断条件，避免虚假唤醒
        if (thread_id == current_id.load()) {
            std::cout << "线程 " << thread_id << " 打印第 " << print_count + 1 << " 次" << std::endl;
            ++print_count;

            // 切换到下一个线程 !!!
            current_id.store((current_id.load() + 1) % NUM_THREADS);

            // 通知所有等待的线程
            cond.notify_all();
        }

        // 为了打印好看，可以实现加一点延时
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    std::cout << "线程 " << thread_id << " 退出" << std::endl;
}

void stop_all_threads()
{
    print_running.store(false);
    cond.notify_all();
}

// 测试函数
void test_multi_thread_print() {
    std::cout << "=============== 多线程交替打印测试开始 ============" << std::endl;

    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back(multi_thread_print, i);
    }

    std::this_thread::sleep_for(std::chrono::seconds(3));
    stop_all_threads();

    for (auto &th :threads) {
        if (th.joinable()) {
            th.join();
        }
    }

    std::cout << "=============== 多线程交替打印测试结束 ============" << std::endl;
}

int main() {
    test_multi_thread_print();
    return 0;
} 