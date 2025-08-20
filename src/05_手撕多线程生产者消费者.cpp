#include <iostream>
#include <algorithm>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

// 手撕多线程生产者消费者实现
std::queue<int> buffer;
std::mutex mtx;
std::condition_variable producer_cv, consumer_cv;
const int MAX_SIZE = 10;
std::atomic<bool> running{true};  // 原子变量运行状态
std::atomic<int> item_counter{0}; // 原子变量生产计数器 避免多生产者生产相同产品

void producer(int id)
{
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);

        // 等待 缓冲区小于阈值需要生产 或 停止运行
        producer_cv.wait(lock, []() {
            return buffer.size() < MAX_SIZE || !running.load();
        });

        if (!running.load()) {
            break;
        }

        if (buffer.size() < MAX_SIZE) { // 再次判断条件避免虚假唤醒
            int item = ++item_counter;
            buffer.push(item);
            std::cout << "生产者" << id << " 生产: " << item << std::endl;

            // 生产后通知：多消费者时用notify_all更好
            consumer_cv.notify_all();
        }

        lock.unlock();

        std::this_thread::sleep_for(std::chrono::milliseconds(40));
    }
    std::cout << "生产者" << id << " 退出" << std::endl;
}

void consumer(int id)
{
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);

        // 等待 有数据 或 停止运行
        consumer_cv.wait(lock, []() { return !buffer.empty() || !running.load(); });

        // 如果停止运行且没有数据，退出
        if (!running.load() && buffer.empty()) {
            break;
        }

        if (!buffer.empty()) { // 再次判断条件避免虚假唤醒
            int val = buffer.front();
            buffer.pop();
            std::cout << "消费者" << id << " 消费: " << val << std::endl;

            // 消费后通知：多生产者时用notify_all
            producer_cv.notify_all();
        }

        // 在出lock作用域之前手动解锁和睡眠的原因：如果让lock出作用域自己释放锁，生产者又会立即获取到锁，消费者不可能获取到（system原因）
        lock.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(40));
    }
    std::cout << "消费者" << id << " 退出" << std::endl;
}

// 优雅停止函数
void stop_all()
{
    running.store(false);
    producer_cv.notify_all();
    consumer_cv.notify_all();
}

// 测试函数
void test_producer_consumer() {
    std::cout << "=============== 多线程生产者消费者测试开始 ============" << std::endl;

    // 创建多个生产者和消费者线程
    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;

    // 启动2个生产者
    for (int i = 0; i < 2; ++i) {
        producers.emplace_back(producer, i);
    }

    // 启动3个消费者
    for (int i = 0; i < 3; ++i) {
        consumers.emplace_back(consumer, i);
    }

    // 运行一段时间后停止
    std::this_thread::sleep_for(std::chrono::seconds(3));
    
    std::cout << "停止所有线程..." << std::endl;
    stop_all();

    // 等待所有线程完成
    for (auto& p : producers) {
        if (p.joinable()) {
            p.join();
        }
    }
    
    for (auto& c : consumers) {
        if (c.joinable()) {
            c.join();
        }
    }

    std::cout << "所有线程已退出" << std::endl;
    std::cout << "=============== 多线程生产者消费者测试结束 ============" << std::endl;
}

int main() {
    test_producer_consumer();
    return 0;
} 