#include<iostream>
template <typename T>
class SimpleUniquePtr
{
private:
    T *ptr = nullptr;
    void release()
    {
        delete ptr;
        ptr = nullptr;
    }

public:
    explicit SimpleUniquePtr(T *ptr)
        : ptr(ptr)
    {
    }

    // 独占指针指针自然需要禁用拷贝构造和赋值运算符
    SimpleUniquePtr(const SimpleUniquePtr &) = delete;
    SimpleUniquePtr &operator=(const SimpleUniquePtr &) = delete;

    // 移动构造 和 移动赋值
    SimpleUniquePtr(SimpleUniquePtr &&other)
        : ptr(other.ptr)
    {
        other.ptr = nullptr;
    }

    SimpleUniquePtr &operator=(SimpleUniquePtr &&other)
    {
        if (this != &other)
        {
            release();
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    ~SimpleUniquePtr()
    {
        release();
    }

    T *get() const
    {
        return ptr;
    }

    void reset(T *other = nullptr)
    {
        release();
        ptr = other;
    }

    // 模拟指针解引用
    T &operator*() const
    {
        return *ptr;
    }

    T *operator->() const
    {
        return ptr;
    }
};

void test_UniquePtr()
{
    // 测试基本构造和析构
    {
        SimpleUniquePtr<int> ptr(new int(42));
        std::cout << "值: " << *ptr << std::endl;
        std::cout << "地址: " << ptr.get() << std::endl;
    } // 这里会自动析构

    // 测试移动构造
    {
        SimpleUniquePtr<int> ptr1(new int(100));
        SimpleUniquePtr<int> ptr2 = std::move(ptr1);
        
        std::cout << "移动后 ptr1: " << ptr1.get() << std::endl; // 应该是 nullptr
        std::cout << "移动后 ptr2: " << *ptr2 << std::endl;     // 应该是 100
    }

    // 测试移动赋值
    {
        SimpleUniquePtr<int> ptr1(new int(200));
        SimpleUniquePtr<int> ptr2(new int(300));
        
        ptr2 = std::move(ptr1);
        std::cout << "赋值后 ptr1: " << ptr1.get() << std::endl; // nullptr
        std::cout << "赋值后 ptr2: " << *ptr2 << std::endl;     // 200
    }

    // 测试 reset
    {
        SimpleUniquePtr<int> ptr(new int(400));
        ptr.reset(new int(500));
        std::cout << "reset后: " << *ptr << std::endl; // 500
        
        ptr.reset(); // 重置为 nullptr
        std::cout << "reset为空后: " << ptr.get() << std::endl; // nullptr
    }

    std::cout << "UniquePtr 测试完成!" << std::endl;
}

int main()
{
    test_UniquePtr();
    return 0;
}
