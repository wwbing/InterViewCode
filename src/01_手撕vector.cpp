#include <iostream>
#include <cstddef>
#include <stdexcept>

// 手撕vector实现
template<typename T>
class my_vec {
private:
    T *arr = nullptr;
    size_t capacity = 0;
    size_t size = 0;

public:
    my_vec(size_t initial_capacity = 2)
        : capacity(initial_capacity), size(0)
    {
        arr = new T[capacity];
    }

    ~my_vec()
    {
        delete[] arr;
    }

    // 拷贝构造
    my_vec(const my_vec &other)
    {
        capacity = other.capacity;
        size = other.size;
        arr = new T[capacity];
        for (size_t i = 0; i < size; ++i)
        {
            arr[i] = other.arr[i];
        }
    }

    // 移动构造
    my_vec(my_vec &&other) noexcept
        : arr(other.arr), capacity(other.capacity), size(other.size)
    {
        other.arr = nullptr;
        other.capacity = 0;
        other.size = 0;
    }

    // 拷贝赋值
    my_vec &operator=(const my_vec &other)
    {
        if (this != &other)
        {
            delete[] arr;
            capacity = other.capacity;
            size = other.size;
            arr = new T[capacity];
            for (size_t i = 0; i < size; ++i)
            {
                arr[i] = other.arr[i];
            }
        }
        return *this;
    }

    // 移动赋值
    my_vec &operator=(my_vec &&other) noexcept
    {
        if (this != &other)
        {
            delete[] arr;
            arr = other.arr;
            capacity = other.capacity;
            size = other.size;
            other.arr = nullptr;
            other.capacity = 0;
            other.size = 0;
        }
        return *this;
    }

    // []
    T &operator[](size_t index)
    {
        if (index < size)
        {
            return arr[index];
        }
        throw std::out_of_range("Index out of range");
    }

    // push_back
    void push_back(const T &value)
    {
        if (size >= capacity)
        {
            capacity *= 2;
            T *tmp = new T[capacity];
            for (size_t i = 0; i < size; ++i)
            {
                tmp[i] = arr[i];
            }
            delete[] arr;
            arr = tmp;
        }
        arr[size++] = value;
    }

    // pop_back
    void pop_back()
    {
        if (size > 0)
        {
            --size;
        }
    }

    // get_size
    size_t get_size() const
    {
        return size;
    }

    // get_capacity
    size_t get_capacity() const
    {
        return capacity;
    }
};

// 测试函数
void test_vector() {
    std::cout << "=============== Vector测试开始 ============" << std::endl;

    my_vec<int> vec;
    std::cout << "初始状态 - cap: " << vec.get_capacity() << ", size: " << vec.get_size() << std::endl;

    // 测试push_back操作
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back(4);
    vec.push_back(5);

    std::cout << "push_back后 - cap: " << vec.get_capacity() << ", size: " << vec.get_size() << std::endl;

    std::cout << "遍历元素: ";
    for (std::size_t i = 0; i < vec.get_size(); ++i)
    {
        std::cout << vec[i] << " ";
    }
    std::cout << std::endl;

    // 测试[]访问
    std::cout << "通过[]访问 - vec[0]: " << vec[0] << ", vec[2]: " << vec[2] << ", vec[4]: " << vec[4] << std::endl;

    // 测试修改元素
    vec[0] = 100;
    vec[2] = 300;
    std::cout << "修改后遍历: ";
    for (std::size_t i = 0; i < vec.get_size(); ++i)
    {
        std::cout << vec[i] << " ";
    }
    std::cout << std::endl;

    // 测试pop_back操作
    vec.pop_back();
    std::cout << "pop_back后 - cap: " << vec.get_capacity() << ", size: " << vec.get_size() << std::endl;
    std::cout << "遍历元素: ";
    for (std::size_t i = 0; i < vec.get_size(); ++i)
    {
        std::cout << vec[i] << " ";
    }
    std::cout << std::endl;

    // 测试多次pop_back
    vec.pop_back();
    vec.pop_back();
    std::cout << "多次pop_back后 - cap: " << vec.get_capacity() << ", size: " << vec.get_size() << std::endl;
    std::cout << "遍历元素: ";
    for (std::size_t i = 0; i < vec.get_size(); ++i)
    {
        std::cout << vec[i] << " ";
    }
    std::cout << std::endl;

    // 测试空vector
    vec.pop_back();
    vec.pop_back();
    std::cout << "vector空后 - cap: " << vec.get_capacity() << ", size: " << vec.get_size() << std::endl;

    // 重新添加元素测试
    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);
    std::cout << "重新添加后 - cap: " << vec.get_capacity() << ", size: " << vec.get_size() << std::endl;
    std::cout << "遍历元素: ";
    for (std::size_t i = 0; i < vec.get_size(); ++i)
    {
        std::cout << vec[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "=============== Vector拷贝测试 ============" << std::endl;

    // 拷贝构造测试
    my_vec<int> vec_copy(vec);
    std::cout << "拷贝构造后 - cap: " << vec_copy.get_capacity() << ", size: " << vec_copy.get_size() << std::endl;
    std::cout << "拷贝vector遍历: ";
    for (std::size_t i = 0; i < vec_copy.get_size(); ++i)
    {
        std::cout << vec_copy[i] << " ";
    }
    std::cout << std::endl;

    // 移动构造测试
    my_vec<int> vec_move(std::move(vec));
    std::cout << "移动构造后 - cap: " << vec_move.get_capacity() << ", size: " << vec_move.get_size() << std::endl;
    std::cout << "移动vector遍历: ";
    for (std::size_t i = 0; i < vec_move.get_size(); ++i)
    {
        std::cout << vec_move[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "=============== Vector测试结束 ============" << std::endl;
}

int main() {
    test_vector();
    return 0;
} 