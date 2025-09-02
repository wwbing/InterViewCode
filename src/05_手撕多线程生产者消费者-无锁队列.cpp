#include <iostream>
#include <atomic>
#include <chrono>
#include <thread>
#include <vector>

// 简单无锁队列节点
struct Node {
    std::atomic<int> data;
    std::atomic<Node*> next;
    std::atomic<bool> valid; // 标记数据是否有效
    
    Node() : data(0), next(nullptr), valid(false) {}
};

// 简单无锁队列实现
class SimpleLockFreeQueue {
private:
    std::atomic<Node*> head;
    std::atomic<Node*> tail;
    
public:
    SimpleLockFreeQueue() {
        Node* dummy = new Node;
        head.store(dummy);
        tail.store(dummy);
    }
    
    ~SimpleLockFreeQueue() {
        while (Node* old_head = head.load()) {
            head.store(old_head->next.load());
            delete old_head;
        }
    }
    
    void enqueue(int value) {
        Node* new_node = new Node;
        new_node->data.store(value);
        new_node->valid.store(true);
        
        while (true) {
            Node* last = tail.load();
            Node* next = last->next.load();
            
            if (last == tail.load()) {
                if (next == nullptr) {
                    // 尝试链接新节点
                    if (last->next.compare_exchange_weak(next, new_node)) {
                        break;
                    }
                } else {
                    // 帮助推进tail
                    tail.compare_exchange_weak(last, next);
                }
            }
        }
        
        // 推进tail指针
        Node* current_tail = tail.load();
        tail.compare_exchange_weak(current_tail, new_node);
    }
    
    bool dequeue(int& result) {
        while (true) {
            Node* first = head.load();
            Node* last = tail.load();
            Node* next = first->next.load();
            
            if (first == head.load()) {
                if (first == last) {
                    if (next == nullptr) {
                        return false; // 队列为空
                    }
                    // 帮助推进tail
                    tail.compare_exchange_weak(last, next);
                } else {
                    if (next == nullptr) {
                        continue;
                    }
                    
                    // 检查数据是否有效
                    if (!next->valid.load()) {
                        continue;
                    }
                    
                    // 尝试推进head
                    if (head.compare_exchange_weak(first, next)) {
                        result = next->data.load();
                        delete first;
                        return true;
                    }
                }
            }
        }
    }
    
    bool empty() const {
        Node* first = head.load();
        Node* last = tail.load();
        return (first == last) && (first->next.load() == nullptr);
    }
};

// 全局变量
SimpleLockFreeQueue buffer;
std::atomic<bool> running{true};
std::atomic<int> item_counter{0};
const int MAX_ITEMS = 50;

void producer(int id) {
    int produced_count = 0;
    while (running.load() && produced_count < MAX_ITEMS / 2) {
        int item = ++item_counter;
        buffer.enqueue(item);
        std::cout << "生产者" << id << " 生产: " << item << std::endl;
        produced_count++;
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << "生产者" << id << " 退出" << std::endl;
}

void consumer(int id) {
    int consumed_count = 0;
    while (running.load()) {
        int item;
        if (buffer.dequeue(item)) {
            std::cout << "消费者" << id << " 消费: " << item << std::endl;
            consumed_count++;
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
    std::cout << "消费者" << id << " 退出，共消费 " << consumed_count << " 个产品" << std::endl;
}

void stop_all() {
    running.store(false);
}

void test_simple_lockfree_queue() {
    std::cout << "=============== 简单无锁队列测试开始 ============" << std::endl;
    
    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;
    
    // 启动2个生产者
    for (int i = 0; i < 2; ++i) {
        producers.emplace_back(producer, i);
    }
    
    // 启动2个消费者
    for (int i = 0; i < 2; ++i) {
        consumers.emplace_back(consumer, i);
    }
    
    // 等待生产者完成
    for (auto& p : producers) {
        if (p.joinable()) {
            p.join();
        }
    }
    
    // 等待消费者消费完剩余产品
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    std::cout << "停止所有线程..." << std::endl;
    stop_all();
    
    for (auto& c : consumers) {
        if (c.joinable()) {
            c.join();
        }
    }
    
    std::cout << "队列是否为空: " << (buffer.empty() ? "是" : "否") << std::endl;
    std::cout << "=============== 简单无锁队列测试结束 ============" << std::endl;
}

int main() {
    test_simple_lockfree_queue();
    return 0;
}