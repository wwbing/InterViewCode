#include <iostream>
#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include <chrono>

// 手撕线程池实现
class ThreadPool
{
  private:
    std::vector<std::thread> workers;        // 工作线程
    std::queue<std::function<void()>> tasks; // 任务队列
    std::mutex queue_mutex;                  // 队列互斥锁
    std::condition_variable condition;       // 条件变量
    std::atomic<bool> stop_flag{false};      // 停止标志

  public:
    ThreadPool(size_t thread_count = 4);
    ~ThreadPool();

    // 提交任务到线程池
    void submit(std::function<void()> task);

    // 停止线程池
    void stop();

    void worker_thread(); // 工作线程函数
};

ThreadPool::ThreadPool(size_t thread_count) : stop_flag(false)
{
    std::cout << "创建线程池，线程数量: " << thread_count << std::endl;

    // 创建工作线程
    for (size_t i = 0; i < thread_count; ++i) {
        workers.emplace_back(&ThreadPool::worker_thread, this);
    }
}

ThreadPool::~ThreadPool()
{
    stop();
}

void ThreadPool::submit(std::function<void()> task)
{
    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        if (stop_flag.load()) {
            std::cout << "线程池已停止，无法提交新任务" << std::endl;
            return;
        }
        tasks.push(task);
        std::cout << "任务已提交，当前队列大小: " << tasks.size() << std::endl;
    }
    condition.notify_one(); // 通知一个工作线程
}

void ThreadPool::stop()
{
    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        stop_flag.store(true);
    }

    condition.notify_all(); // 唤醒所有工作线程

    // 等待所有线程结束
    for (auto &worker : workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
    std::cout << "线程池已停止" << std::endl;
}

void ThreadPool::worker_thread()
{
    std::cout << "工作线程 " << std::this_thread::get_id() << " 启动" << std::endl;

    while (true) {

        std::unique_lock<std::mutex> lock(queue_mutex);

        // 等待任务或停止信号
        condition.wait(lock, [this]() {
            return !tasks.empty() || stop_flag.load();
        });

        // 如果外部停止，退出
        if (stop_flag.load() && tasks.empty()) {
            break;
        }

        if (!tasks.empty()) { // 再次判断避免虚假唤醒
            std::function<void()> task;

            // 取出任务
            task = tasks.front();
            tasks.pop();
            std::cout << "线程 " << std::this_thread::get_id() << " 取到任务，剩余任务: " << tasks.size() << std::endl;

            lock.unlock();

            // 执行任务
            try {
                task();
            } catch (const std::exception &e) {
                std::cout << "任务执行异常: " << e.what() << std::endl;
            }
        }
    }

    std::cout << "工作线程 " << std::this_thread::get_id() << " 退出" << std::endl;
}

// 测试函数
void test_thread_pool()
{
    std::cout << "\n=== 线程池测试开始 ===" << std::endl;

    ThreadPool pool(3); // 创建3个工作线程的线程池

    // 提交一些测试任务
    for (int i = 1; i <= 8; ++i) {
        pool.submit([i]() -> void {
            std::cout << "执行任务 " << i << " (线程: " << std::this_thread::get_id() << ")" << std::endl;

            // 模拟任务执行时间
            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            std::cout << "任务 " << i << " 完成" << std::endl;
        });
    }

    // 让任务运行一段时间
    std::this_thread::sleep_for(std::chrono::seconds(3));

    std::cout << "\n准备停止线程池..." << std::endl;
    // 析构函数会自动调用stop()

    std::cout << "=== 线程池测试结束 ===" << std::endl;
}

int main() {
    test_thread_pool();
    return 0;
} 