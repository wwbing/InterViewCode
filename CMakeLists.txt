cmake_minimum_required(VERSION 3.20)

project(面试手撕)


message(STATUS "========== 开始配置 ${PROJECT_NAME} 项目 ==========")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # 生成 compile_commands.json 供 clangd 使用

# C++20 标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 调试信息
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")

add_executable(main.out src/main.cpp)

# 链接pthread库
find_package(Threads REQUIRED)
target_link_libraries(main.out Threads::Threads)

