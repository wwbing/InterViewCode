#include <iostream>
#include <cstddef>
#include <stdexcept>

// 手撕queue实现
template <typename T>
class quee
{
private:
    size_t capacity = 0;
    size_t size = 0;
    size_t front = 0;
    size_t tail = 0;
    T *arr = nullptr;

public:
    quee(size_t initial_capacity = 2)
        : capacity(initial_capacity), size(0), front(0), tail(0)
    {
        arr = new T[capacity];
    }
    
    ~quee()
    {
        delete [] arr;
    }

    quee(const quee &other)
    {
        capacity = other.capacity;
        size = other.size;
        front = other.front;
        tail = other.tail;

        arr = new T[capacity];
        if (front < tail)
        {
            for (int i = front; i < tail; ++i)
            {
                arr[i] = other.arr[i];
            }
        }
        else
        {
            for (size_t i = 0; i < tail; ++i)
            {
                arr[i] = other.arr[i];
            }
            for (size_t i = front; i < capacity; ++i)
            {
                arr[i] = other.arr[i];
            }
        }
    }

    quee &operator=(const quee &other)
    {
        if (this != &other)
        {
            delete[] arr;
            capacity = other.capacity;
            size = other.size;
            front = other.front;
            tail = other.tail;

            arr = new T[capacity];
            if (front < tail)
            {
                for (size_t i = front; i < tail; ++i)
                {
                    arr[i] = other.arr[i];
                }
            }
            else
            {
                for (size_t i = 0; i < tail; ++i)
                {
                    arr[i] = other.arr[i];
                }
                for (size_t i = front; i < capacity; ++i)
                {
                    arr[i] = other.arr[i];
                }
            }
        }
        return *this;
    }

    void push(T &val)
    {
        if (capacity == size)
        {
            capacity *= 2;
            T *tmp = new T[capacity];

            if (front < tail)
            {
                // 顺序满了(front=0, tail=size)：直接一次拷贝就行
                for (size_t i = front; i < tail; ++i)
                {
                    tmp[i] = arr[i];
                }
            }
            else
            {
                // 环形满了：拷贝过去要重置顺序
                for (size_t i = 0; i < size; ++i)
                {
                    tmp[i] = arr[(front + i) % (capacity / 2)];
                }
            }

            // 不管是哪种存储，拷贝到tmp都是顺序的
            front = 0;
            tail = size;

            delete[] arr;
            arr = tmp;
            tmp = nullptr;
        }
        arr[tail] = val;
        tail = (tail + 1) % capacity;
        ++size;
    }

    void pop()
    {
        if (size > 0)
        {
            front = (front + 1) % capacity;
            --size;
        }
    }

    T &get_front()
    {
        if (size > 0)
        {
            return arr[front];
        }
        throw std::out_of_range("index out of range");
    }

    size_t get_size()
    {
        return size;
    }
    size_t get_capacity()
    {
        return capacity;
    }

};

// 测试函数
void test_queue() {
    std::cout << "=============== Queue测试开始 ============" << std::endl;

    quee<int> q;
    std::cout << "初始状态 - cap: " << q.get_capacity() << ", size: " << q.get_size() << std::endl;

    // 测试push操作
    int val1 = 1, val2 = 2, val3 = 3, val4 = 4, val5 = 5;
    q.push(val1);
    q.push(val2);
    q.push(val3);
    q.push(val4);
    q.push(val5);

    std::cout << "push后 - cap: " << q.get_capacity() << ", size: " << q.get_size() << std::endl;

    // 测试get_front操作
    std::cout << "队首元素: " << q.get_front() << std::endl;

    // 测试pop操作
    q.pop();
    std::cout << "pop后 - size: " << q.get_size() << ", 队首元素: " << q.get_front() << std::endl;

    q.pop();
    q.pop();
    std::cout << "多次pop后 - size: " << q.get_size() << ", 队首元素: " << q.get_front() << std::endl;

    // 测试空队列
    q.pop();
    q.pop();
    std::cout << "队列空后 - size: " << q.get_size() << std::endl;

    // 测试空队列get_front操作
    try
    {
        std::cout << "尝试访问空队列..." << std::endl;
        int invalid = q.get_front();
        std::cout << "未抛出异常，这是错误的！" << std::endl;
    }
    catch (const std::out_of_range& e)
    {
        std::cout << "正确捕获空队列异常: " << e.what() << std::endl;
    }

    // 重新添加元素测试
    int new_val1 = 10, new_val2 = 20, new_val3 = 30;
    q.push(new_val1);
    q.push(new_val2);
    q.push(new_val3);
    std::cout << "重新添加后 - cap: " << q.get_capacity() << ", size: " << q.get_size() << std::endl;
    std::cout << "队首元素: " << q.get_front() << std::endl;

    std::cout << "=============== Queue拷贝测试 ============" << std::endl;

    // 拷贝构造测试
    quee<int> q_copy(q);
    std::cout << "拷贝构造后 - cap: " << q_copy.get_capacity() << ", size: " << q_copy.get_size() << std::endl;
    std::cout << "拷贝队列队首: " << q_copy.get_front() << std::endl;

    // 赋值操作测试
    quee<int> q_assign(3);
    q_assign = q;
    std::cout << "赋值后 - cap: " << q_assign.get_capacity() << ", size: " << q_assign.get_size() << std::endl;
    std::cout << "赋值队列队首: " << q_assign.get_front() << std::endl;

    // 验证拷贝的独立性
    q.pop();
    std::cout << "原队列pop后 - size: " << q.get_size() << ", 队首: " << q.get_front() << std::endl;
    std::cout << "拷贝队列未变 - size: " << q_copy.get_size() << ", 队首: " << q_copy.get_front() << std::endl;

    std::cout << "=============== Queue环形队列测试 ============" << std::endl;

    // 测试环形队列特性
    quee<int> ring_q(3);
    int r1 = 10, r2 = 20, r3 = 30;
    ring_q.push(r1);
    ring_q.push(r2);
    ring_q.push(r3);
    std::cout << "填满小容量队列 - cap: " << ring_q.get_capacity() << ", size: " << ring_q.get_size() << std::endl;

    // pop一些元素，然后再push，测试环形特性
    ring_q.pop();
    ring_q.pop();
    std::cout << "pop两次后 - size: " << ring_q.get_size() << ", 队首: " << ring_q.get_front() << std::endl;

    int r4 = 40, r5 = 50, r6 = 60;
    ring_q.push(r4);
    ring_q.push(r5);
    ring_q.push(r6); // 这里会触发扩容
    std::cout << "重新push三个元素后 - cap: " << ring_q.get_capacity() << ", size: " << ring_q.get_size() << std::endl;

    std::cout << "环形队列最终内容: ";
    while (ring_q.get_size() > 0)
    {
        std::cout << ring_q.get_front() << " ";
        ring_q.pop();
    }
    std::cout << std::endl;

    std::cout << "=============== Queue测试结束 ============" << std::endl;
}

int main() {
    test_queue();
    return 0;
} 