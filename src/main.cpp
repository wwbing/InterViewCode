#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
using namespace std;

// 手撕单例模式实现

// 饿汉式单例（天生线程安全）
class EagerSingleton {
private:
    static EagerSingleton instance;  // 静态成员，程序启动时创建

    // 私有构造函数
    EagerSingleton() {
        std::cout << "饿汉式单例创建" << std::endl;
    }

    // 禁止拷贝和赋值
    EagerSingleton(const EagerSingleton&) = delete;
    EagerSingleton& operator=(const EagerSingleton&) = delete;

public:
    static EagerSingleton& getInstance() {
        return instance;
    }

    void doSomething() {
        std::cout << "饿汉式单例执行操作" << std::endl;
    }
};

// 懒汉式单例（互斥锁线程安全）
class LazySingleton {
private:
    static LazySingleton* instance;
    static std::mutex mtx;

    // 私有构造函数
    LazySingleton() {
        std::cout << "懒汉式单例创建" << std::endl;
    }

    // 禁止拷贝和赋值
    LazySingleton(const LazySingleton&) = delete;
    LazySingleton& operator=(const LazySingleton&) = delete;

public:
    static LazySingleton* getInstance() {
        if (instance == nullptr) {
            std::lock_guard<std::mutex> lock(mtx);
            if (instance == nullptr) {  // 双重检查
                instance = new LazySingleton();
            }
        }
        return instance;
    }

    void doSomething() {
        std::cout << "懒汉式单例执行操作" << std::endl;
    }
};

// 懒汉式单例（once_flag线程安全）
class LazySingleton2 {
private:
    static LazySingleton2* instance;
    static std::once_flag init_flag;

    // 私有构造函数
    LazySingleton2() {
        std::cout << "once_flag懒汉式单例创建" << std::endl;
    }

    // 禁止拷贝和赋值
    LazySingleton2(const LazySingleton2&) = delete;
    LazySingleton2& operator=(const LazySingleton2&) = delete;

public:
    static LazySingleton2* getInstance() {
        std::call_once(init_flag, []() {
            instance = new LazySingleton2();
        });
        return instance;
    }

    void doSomething() {
        std::cout << "once_flag懒汉式单例执行操作" << std::endl;
    }
};

// 饿汉式单例的静态成员定义
EagerSingleton EagerSingleton::instance;

// 互斥锁懒汉式单例的静态成员定义
LazySingleton *LazySingleton::instance = nullptr;
std::mutex LazySingleton::mtx;

// once_flag懒汉式单例的静态成员定义
LazySingleton2* LazySingleton2::instance = nullptr;
std::once_flag LazySingleton2::init_flag;

// 测试函数
void test_singleton()
{
    std::cout << "\n=== 单例模式测试开始 ===" << std::endl;

    // 懒汉式单例测试
    {
        std::cout << "--- 懒汉式单例测试 ---" << std::endl;

        LazySingleton *lazy1 = LazySingleton::getInstance();
        LazySingleton *lazy2 = LazySingleton::getInstance();

        std::cout << "lazy1地址: " << lazy1 << std::endl;
        std::cout << "lazy2地址: " << lazy2 << std::endl;
        std::cout << "是否为同一实例: " << (lazy1 == lazy2 ? "是" : "否") << std::endl;

        lazy1->doSomething();
        lazy2->doSomething();
    }

    // 饿汉式单例测试
    {
        std::cout << "\n--- 饿汉式单例测试 ---" << std::endl;

        EagerSingleton &eager1 = EagerSingleton::getInstance();
        EagerSingleton &eager2 = EagerSingleton::getInstance();

        std::cout << "eager1地址: " << &eager1 << std::endl;
        std::cout << "eager2地址: " << &eager2 << std::endl;
        std::cout << "是否为同一实例: " << (&eager1 == &eager2 ? "是" : "否") << std::endl;

        eager1.doSomething();
        eager2.doSomething();
    }

    // once_flag懒汉式单例测试
    {
        std::cout << "\n--- once_flag懒汉式单例测试 ---" << std::endl;

        LazySingleton2* lazy1 = LazySingleton2::getInstance();
        LazySingleton2* lazy2 = LazySingleton2::getInstance();

        std::cout << "lazy1地址: " << lazy1 << std::endl;
        std::cout << "lazy2地址: " << lazy2 << std::endl;
        std::cout << "是否为同一实例: " << (lazy1 == lazy2 ? "是" : "否") << std::endl;

        lazy1->doSomething();
        lazy2->doSomething();
    }

    // 多线程测试（懒汉式）
    {
        std::cout << "\n--- 多线程测试（懒汉式） ---" << std::endl;

        std::vector<std::thread> threads;
        std::vector<LazySingleton *> instances(5);

        // 创建5个线程同时获取单例
        for (int i = 0; i < 5; ++i) {
            threads.emplace_back([&instances, i]() {
                instances[i] = LazySingleton::getInstance();
                std::cout << "线程" << i << "获取实例地址: " << instances[i] << std::endl;
            });
        }

        // 等待所有线程完成
        for (auto &t : threads) {
            t.join();
        }

        // 验证所有实例是否相同
        bool all_same = true;
        for (int i = 1; i < 5; ++i) {
            if (instances[i] != instances[0]) {
                all_same = false;
                break;
            }
        }
        std::cout << "多线程获取的实例是否相同: " << (all_same ? "是" : "否") << std::endl;
    }

    // 多线程测试（once_flag懒汉式）
    {
        std::cout << "\n--- 多线程测试（once_flag懒汉式） ---" << std::endl;

        std::vector<std::thread> threads;
        std::vector<LazySingleton2*> instances(5);

        // 创建5个线程同时获取单例
        for (int i = 0; i < 5; ++i) {
            threads.emplace_back([&instances, i]() {
                instances[i] = LazySingleton2::getInstance();
                std::cout << "线程" << i << "获取once_flag实例地址: " << instances[i] << std::endl;
            });
        }

        // 等待所有线程完成
        for (auto& t : threads) {
            t.join();
        }

        // 验证所有实例是否相同
        bool all_same = true;
        for (int i = 1; i < 5; ++i) {
            if (instances[i] != instances[0]) {
                all_same = false;
                break;
            }
        }
        std::cout << "多线程获取的once_flag实例是否相同: " << (all_same ? "是" : "否") << std::endl;
    }

    std::cout << "=== 单例模式测试结束 ===" << std::endl;
}
struct alignas(32) data0 {
        int a = 1;
        double b = 2.0;
};

int main()
{
    cout << sizeof(int) << endl;
    cout << sizeof(double) << endl;
    cout<<alignof(data0)<<endl;
    cout<<sizeof(data0)<<endl;
    return 0;
} 