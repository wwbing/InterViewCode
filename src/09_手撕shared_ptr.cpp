#include <functional>
#include <iostream>
#include <utility>

template <typename T>
class SimpleSharedPtr
{
private:
    T *ptr = nullptr;
    int *count = nullptr; // 计数器使用指针类型的原因：多个对象共享所指向的地址
    void release()
    {
        if (count)
        {
            --(*count);
            if (*count == 0)
            {
                delete ptr;
                delete count;
            }
        }
    }

public:
    SimpleSharedPtr(T *ptr)
        : ptr(ptr), count(ptr ? new int(1) : nullptr)
    {
    }

    // 和UniquePtr相比，sharedPtr可以进行拷贝构造和赋值且会导致count++
    SimpleSharedPtr(const SimpleSharedPtr &other)
        : ptr(other.ptr),count(other.count)
    {
        if (count)
        {
            (*count)++;
        }
    }

    SimpleSharedPtr &operator=(const SimpleSharedPtr& other)
    {
        if (this != &other)
        {
            release();
            ptr = other.ptr;
            count = other.count;
            if (count) {
                (*count)++;
            }
        }
        return *this;
    }

    SimpleSharedPtr(SimpleSharedPtr &&other)
        : ptr(other.ptr), count(other.count)
    {
        other.ptr = nullptr;
        other.count = nullptr;
    }

    SimpleSharedPtr &operator=(SimpleSharedPtr &&other)
    {
        if (this != &other)
        {
            release();
            count = other.count;
            ptr = other.ptr;

            other.count = nullptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    ~SimpleSharedPtr()
    {
        release();
    }

    T &operator*()
    {
        if (ptr)
        {
            return *ptr;
        }
        throw std::runtime_error("Dereferencing null pointer");
    }

    T *operator->()
    {
        return ptr;  // 可以返回nullptr
    }

    T *get()
    {
        return ptr;  // 可以返回nullptr
    }

    int use_count()
    {
        if (count)
        {
            return (*count);
        }
        return 0;  // 返回0而不是抛异常
    }

    void reset(T *p)
    {
        release();
        ptr = p;
        count = ptr ? new int(1) : nullptr;
    }

};

void test_SharedPtr()
{
    // 测试基本构造和引用计数
    {
        SimpleSharedPtr<int> ptr1(new int(42));
        std::cout << "ptr1 值: " << *ptr1 << ", 引用计数: " << ptr1.use_count() << std::endl;
        
        // 测试拷贝构造
        SimpleSharedPtr<int> ptr2 = ptr1;
        std::cout << "拷贝后 ptr1 引用计数: " << ptr1.use_count() << std::endl;
        std::cout << "拷贝后 ptr2 引用计数: " << ptr2.use_count() << std::endl;
        
        // 测试拷贝赋值
        SimpleSharedPtr<int> ptr3(new int(100));
        ptr3 = ptr1;
        std::cout << "赋值后引用计数: " << ptr1.use_count() << std::endl;
    }

    // 测试移动构造和移动赋值
    {
        SimpleSharedPtr<int> ptr1(new int(200));
        std::cout << "移动前 ptr1 引用计数: " << ptr1.use_count() << std::endl;
        
        SimpleSharedPtr<int> ptr2 = std::move(ptr1);
        std::cout << "移动后 ptr1 引用计数: " << ptr1.use_count() << std::endl;
        std::cout << "移动后 ptr2 引用计数: " << ptr2.use_count() << std::endl;
    }

    // 测试 reset
    {
        SimpleSharedPtr<int> ptr1(new int(300));
        SimpleSharedPtr<int> ptr2 = ptr1;
        std::cout << "reset前引用计数: " << ptr1.use_count() << std::endl;
        
        ptr1.reset(new int(400));
        std::cout << "ptr1 reset后: " << *ptr1 << ", 引用计数: " << ptr1.use_count() << std::endl;
        std::cout << "ptr2 值: " << *ptr2 << ", 引用计数: " << ptr2.use_count() << std::endl;
    }

    std::cout << "SharedPtr 测试完成!" << std::endl;
}

int main()
{
    test_SharedPtr();
    return 0;
}
