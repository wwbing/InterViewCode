#include <iostream>
#include <cstddef>
#include <exception>
#include <iterator>
#include <stdexcept>

// 手撕stack实现
template <typename T>
class my_st
{
private:
    T *arr = nullptr;
    std::size_t size = 0;
    std::size_t capacity = 0;

public:
    // 构造
    my_st(std::size_t initial_capcity = 2)
        : capacity(initial_capcity), arr(new T[initial_capcity]), size(0)
    {
    }

    ~my_st()
    {
        if (arr)
        {
            delete[] arr;
        }
    }

    // 拷贝
    my_st(const my_st &other)
    {
        size = other.size;
        capacity = other.capacity;
        arr = new T[capacity];

        for (std::size_t i = 0; i < size; ++i)
        {
            arr[i] = other.arr[i];
        }
    }

    // 赋值
    my_st &operator=(const my_st &other)
    {
        if (this != &other)
        {
            delete[] arr;
            capacity = other.capacity;
            size = other.size;
            arr = new T[capacity];

            for (std::size_t i = 0; i < size; ++i)
            {
                arr[i] = other.arr[i];
            }
        }
        return *this;
    }

    // 移动构造
    my_st(my_st &&other)
    {
        capacity = other.capacity;
        size = other.size;
        arr = other.arr;

        other.arr = nullptr;
        other.size = 0;
        other.capacity = 0;
    }

    // 移动赋值
    my_st &operator=(my_st &&other)
    {
        if (this != &other)
        {
            delete[] arr;
            capacity = other.capacity;
            size = other.size;
            arr = other.arr;

            other.arr = nullptr;
            other.size = 0;
            other.capacity = 0;
        }
        return *this;
    }

    // size
    std::size_t get_size()
    {
        return size;
    }

    // capacity
    std::size_t get_capacity()
    {
        return capacity;
    }

    // push
    void push(T &val)
    {
        if (capacity == size)
        {
            capacity *= 2;
            T *tmp = new T[capacity];

            for (std::size_t i = 0; i < size; ++i)
            {
                tmp[i] = arr[i];
            }
            delete[] arr;
            arr = tmp;
            tmp = nullptr;
        }
        arr[size++] = val;
    }

    // pop
    void pop()
    {
        if (size > 0)
        {
            --size;
        }
    }

    // top
    T &top()
    {
        if (size > 0)
        {
            return arr[size - 1]; // top只是取最上面的元素并不会出栈
        }
        else
        {
            throw std::out_of_range("stack is empty");
        }
    }
};

// 测试函数
void test_stack() {
    std::cout << "=============== Stack测试开始 ============" << std::endl;

    my_st<int> st;
    std::cout << "初始状态 - cap: " << st.get_capacity() << ", size: " << st.get_size() << std::endl;

    // 测试push操作
    int val1 = 1, val2 = 2, val3 = 3, val4 = 4, val5 = 5;
    st.push(val1);
    st.push(val2);
    st.push(val3);
    st.push(val4);
    st.push(val5);

    std::cout << "push后 - cap: " << st.get_capacity() << ", size: " << st.get_size() << std::endl;

    // 测试top操作
    std::cout << "栈顶元素: " << st.top() << std::endl;

    // 测试pop操作
    st.pop();
    std::cout << "pop后 - size: " << st.get_size() << ", 栈顶元素: " << st.top() << std::endl;

    st.pop();
    st.pop();
    std::cout << "多次pop后 - size: " << st.get_size() << ", 栈顶元素: " << st.top() << std::endl;

    // 测试空栈
    st.pop();
    st.pop();
    std::cout << "栈空后 - size: " << st.get_size() << std::endl;

    // 测试空栈top操作
    try
    {
        std::cout << "尝试访问空栈..." << std::endl;
        int invalid = st.top();
        std::cout << "未抛出异常，这是错误的！" << std::endl;
    }
    catch (const std::out_of_range& e)
    {
        std::cout << "正确捕获空栈异常: " << e.what() << std::endl;
    }

    // 重新添加元素测试
    int new_val1 = 10, new_val2 = 20, new_val3 = 30;
    st.push(new_val1);
    st.push(new_val2);
    st.push(new_val3);
    std::cout << "重新添加后 - cap: " << st.get_capacity() << ", size: " << st.get_size() << std::endl;
    std::cout << "栈顶元素: " << st.top() << std::endl;

    std::cout << "=============== Stack拷贝测试 ============" << std::endl;

    // 拷贝构造测试
    my_st<int> st_copy(st);
    std::cout << "拷贝构造后 - cap: " << st_copy.get_capacity() << ", size: " << st_copy.get_size() << std::endl;
    std::cout << "拷贝栈顶元素: " << st_copy.top() << std::endl;

    // 移动构造测试
    my_st<int> st_move(std::move(st));
    std::cout << "移动构造后 - cap: " << st_move.get_capacity() << ", size: " << st_move.get_size() << std::endl;
    std::cout << "移动栈顶元素: " << st_move.top() << std::endl;

    std::cout << "=============== Stack测试结束 ============" << std::endl;
}

int main() {
    test_stack();
    return 0;
} 