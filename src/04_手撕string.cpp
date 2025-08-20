#include <iostream>
#include <cstddef>
#include <stdexcept>

// 手撕string实现
class str
{
private:
    char *arr = nullptr;
    size_t capacity = 0;
    size_t size = 0;

public:
    str(size_t initial_capacity = 2)
        : capacity(initial_capacity), size(0)
    {
        arr = new char[capacity];
    }

    ~str()
    {
        delete[] arr;
    }

    // 拷贝
    str(const str &other)
    {
        capacity = other.capacity;
        size = other.size;
        arr = new char[capacity];
        for (size_t i = 0; i < size; ++i)
        {
            arr[i] = other.arr[i];
        }
    }
    // =
    str &operator=(const str &other)
    {
        if (this != &other)
        {
            delete[] arr;
            capacity = other.capacity;
            size = other.size;
            arr = new char[capacity];
            for (size_t i = 0; i < size; ++i)
            {
                arr[i] = other.arr[i];
            }
        }
        return *this;
    }

    // []
    char &operator[](size_t index)
    {
        if (index < size)
        {
            return arr[index];
        }
        throw std::out_of_range("Index out of range");
    }

    str &operator+=(str &other)
    {
        if (size + other.size >= capacity)
        {
            capacity *= 2;
            char *tmp = new char[capacity];

            for (size_t i = 0; i < size; ++i)
            {
                tmp[i] = arr[i];
            }

            for (size_t i = 0; i < other.size; ++i)
            {
                tmp[size + i] = other.arr[i];
            }

            size += other.size;

            delete[] arr;
            arr = tmp;
            tmp = nullptr;
        }
        else
        {
            for (size_t i = 0; i < other.size; ++i)
            {
                arr[size + i] = other.arr[i];
            }
            size += other.size;
        }

        return *this;
    }

    void append(char &val)
    {
        if (capacity == size)
        {
            capacity *= 2;
            char *tmp = new char[capacity];

            for (size_t i=0;i<size;++i)
            {
                tmp[i] = arr[i];
            }

            delete[] arr;
            arr = tmp;
            tmp = nullptr;
        }
        arr[size++] = val;
    }

    // get_size
    size_t get_size()
    {
        return size;
    }

    // get_capacity
    size_t get_capacity()
    {
        return capacity;
    }
};

// 测试函数
void test_string() {
    std::cout << "=============== String测试开始 ============" << std::endl;

    str s1(4);
    std::cout << "初始状态 - cap: " << s1.get_capacity() << ", size: " << s1.get_size() << std::endl;

    // 测试append操作
    char c1 = 'H', c2 = 'e', c3 = 'l', c4 = 'l', c5 = 'o';
    s1.append(c1);
    s1.append(c2);
    s1.append(c3);
    s1.append(c4);
    s1.append(c5);

    std::cout << "append后 - cap: " << s1.get_capacity() << ", size: " << s1.get_size() << std::endl;

    // 测试[]访问
    std::cout << "通过[]访问字符串: ";
    for (size_t i = 0; i < s1.get_size(); ++i)
    {
        std::cout << s1[i];
    }
    std::cout << std::endl;

    // 测试修改字符
    s1[0] = 'h';
    std::cout << "修改首字母后: ";
    for (size_t i = 0; i < s1.get_size(); ++i)
    {
        std::cout << s1[i];
    }
    std::cout << std::endl;

    // 测试+=操作
    str s2(2);
    char c6 = ' ', c7 = 'W', c8 = 'o', c9 = 'r', c10 = 'l', c11 = 'd';
    s2.append(c6);
    s2.append(c7);
    s2.append(c8);
    s2.append(c9);
    s2.append(c10);
    s2.append(c11);

    std::cout << "s2内容: ";
    for (size_t i = 0; i < s2.get_size(); ++i)
    {
        std::cout << s2[i];
    }
    std::cout << std::endl;

    s1 += s2;
    std::cout << "s1 += s2后 - cap: " << s1.get_capacity() << ", size: " << s1.get_size() << std::endl;
    std::cout << "合并后内容: ";
    for (size_t i = 0; i < s1.get_size(); ++i)
    {
        std::cout << s1[i];
    }
    std::cout << std::endl;

    std::cout << "=============== String拷贝测试 ============" << std::endl;

    // 拷贝构造测试
    str s3(s1);
    std::cout << "拷贝构造后 - cap: " << s3.get_capacity() << ", size: " << s3.get_size() << std::endl;
    std::cout << "拷贝字符串内容: ";
    for (size_t i = 0; i < s3.get_size(); ++i)
    {
        std::cout << s3[i];
    }
    std::cout << std::endl;

    // 赋值操作测试
    str s4(2);
    s4 = s1;
    std::cout << "赋值后 - cap: " << s4.get_capacity() << ", size: " << s4.get_size() << std::endl;
    std::cout << "赋值字符串内容: ";
    for (size_t i = 0; i < s4.get_size(); ++i)
    {
        std::cout << s4[i];
    }
    std::cout << std::endl;

    // 验证拷贝的独立性
    s1[0] = 'X';
    std::cout << "修改原字符串首字母后: ";
    for (size_t i = 0; i < s1.get_size(); ++i)
    {
        std::cout << s1[i];
    }
    std::cout << std::endl;
    std::cout << "拷贝字符串未变: ";
    for (size_t i = 0; i < s3.get_size(); ++i)
    {
        std::cout << s3[i];
    }
    std::cout << std::endl;

    std::cout << "=============== String边界测试 ============" << std::endl;

    // 测试空字符串
    str empty_str(1);
    std::cout << "空字符串 - cap: " << empty_str.get_capacity() << ", size: " << empty_str.get_size() << std::endl;

    // 测试单字符
    char single = 'A';
    empty_str.append(single);
    std::cout << "单字符字符串 - size: " << empty_str.get_size() << ", 内容: " << empty_str[0] << std::endl;

    // 测试大量字符
    str large_str(2);
    std::cout << "大量字符测试 - 添加26个字母" << std::endl;
    for (char c = 'a'; c <= 'z'; ++c)
    {
        large_str.append(c);
    }
    std::cout << "添加完成 - cap: " << large_str.get_capacity() << ", size: " << large_str.get_size() << std::endl;
    std::cout << "字母表内容: ";
    for (size_t i = 0; i < large_str.get_size(); ++i)
    {
        std::cout << large_str[i];
    }
    std::cout << std::endl;

    // 测试扩容机制
    str expand_str(2);
    std::cout << "扩容测试 - 初始cap: " << expand_str.get_capacity() << std::endl;
    for (int i = 0; i < 10; ++i)
    {
        char test_c = '0' + i;
        expand_str.append(test_c);
        std::cout << "添加字符 " << test_c << " 后 - cap: " << expand_str.get_capacity() 
                 << ", size: " << expand_str.get_size() << std::endl;
    }

    std::cout << "=============== String异常测试 ============" << std::endl;

    // 测试越界访问
    try
    {
        std::cout << "尝试访问越界索引..." << std::endl;
        char invalid = empty_str[1]; // 应该抛出异常
        std::cout << "未抛出异常，这是错误的！" << std::endl;
    }
    catch (const std::out_of_range& e)
    {
        std::cout << "正确捕获越界异常: " << e.what() << std::endl;
    }

    // 测试空字符串访问
    str zero_str(1);
    try
    {
        std::cout << "尝试访问空字符串..." << std::endl;
        char invalid = zero_str[0]; // 应该抛出异常
        std::cout << "未抛出异常，这是错误的！" << std::endl;
    }
    catch (const std::out_of_range& e)
    {
        std::cout << "正确捕获空字符串异常: " << e.what() << std::endl;
    }

    std::cout << "=============== String测试结束 ============" << std::endl;
}

int main() {
    test_string();
    return 0;
} 