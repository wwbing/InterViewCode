# GDB快速调试指南

## 1. 编译配置

### 基本调试编译
```bash
# 生成调试信息
gcc -g -O0 source.c -o program
g++ -g -O0 source.cpp -o program

# CMake配置
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
```

### 参数说明
- `-g`: 生成调试信息
- `-O0`: 关闭优化，便于调试
- `-Wall`: 显示所有警告

## 2. Core文件生成

### 启用Core文件
```bash
# 设置core文件大小限制
ulimit -c unlimited

# 永久设置（添加到~/.bashrc）
echo "ulimit -c unlimited" >> ~/.bashrc
source ~/.bashrc
```

### 设置Core文件路径
```bash
# 设置core文件路径和命名
echo "/tmp/core-%e-%p-%t" | sudo tee /proc/sys/kernel/core_pattern

# 永久设置
echo "kernel.core_pattern = /tmp/core-%e-%p-%t" | sudo tee -a /etc/sysctl.conf
sudo sysctl -p
```

### Core文件命名格式
- `%e`: 可执行文件名
- `%p`: 进程ID
- `%t`: 时间戳

## 3. 调试Core文件

### 启动调试
```bash
# 调试core文件
gdb program core

# 查看崩溃调用栈
(gdb) bt

# 查看崩溃位置
(gdb) frame 0

# 查看变量值
(gdb) print variable
```

### 调试正在运行的进程
```bash
# 附加到进程
gdb program pid

# 分离进程
(gdb) detach
```

## 4. GDB常用指令

### 基本操作
```bash
gdb program          # 启动调试
run                  # 运行程序
run arg1 arg2        # 带参数运行
quit                 # 退出GDB
```

### 断点操作
```bash
break main           # 在main函数设置断点
break 10             # 在第10行设置断点
break filename:line  # 在指定文件行设置断点
info breakpoints     # 查看所有断点
delete 1             # 删除断点1
clear                # 删除所有断点
```

### 程序控制
```bash
continue             # 继续运行到下一个断点
next                 # 单步执行（不进入函数）
step                 # 单步执行（进入函数）
finish               # 运行到当前函数结束
```

### 查看信息
```bash
print variable       # 打印变量值
print *ptr           # 打印指针指向的值
display variable     # 每次停止时自动打印变量
info locals          # 查看局部变量
info args            # 查看函数参数
backtrace            # 查看调用栈
frame 2              # 切换到调用栈第2层
```

### 内存操作
```bash
x/10x ptr            # 以16进制查看10个字节
x/s ptr              # 查看字符串
x/10i ptr            # 查看10条汇编指令
set variable = value # 修改变量值
```

### 条件断点
```bash
break 10 if i == 5   # 条件断点
condition 1 i > 10   # 修改断点条件
```

## 5. 调试示例

### 段错误调试
```bash
# 编译
gcc -g -O0 segfault.c -o segfault

# 运行生成core文件
./segfault

# 调试core文件
gdb segfault core
(gdb) bt
(gdb) frame 0
(gdb) print *ptr
```

### 实时调试
```bash
gdb program
(gdb) break main
(gdb) run
(gdb) next
(gdb) print variable
(gdb) continue
```

## 6. 常见问题

### 没有调试符号
```bash
# 问题：No debugging symbols found
# 解决：重新编译时添加-g选项
gcc -g -O0 source.c -o program
```

### 变量被优化掉
```bash
# 问题：变量不可见
# 解决：使用-O0选项关闭优化
gcc -g -O0 source.c -o program
```

## 7. 实用技巧

### 输出控制
```bash
set pagination off    # 关闭分页
set print array on    # 完整打印数组
set print pretty on   # 美化输出
```

### 日志记录
```bash
set logging on        # 开启日志记录
set logging file debug.log  # 设置日志文件
```

## 总结

1. **编译时加-g -O0**：生成调试信息，关闭优化
2. **设置ulimit -c unlimited**：启用core文件生成
3. **gdb program core**：调试崩溃程序
4. **bt + frame 0**：查看崩溃位置
5. **print + display**：查看变量值
6. **break + continue**：设置断点控制执行

掌握这些基本操作就能解决大部分调试问题！ 