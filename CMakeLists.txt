cmake_minimum_required(VERSION 3.10)
project(Backuptool VERSION 1.0.0)

# 设置C++标准为17，并要求必须支持
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# 关闭一些编译器扩展，提高可移植性
set(CMAKE_CXX_EXTENSIONS OFF)

# 添加可执行文件的输出目录（可选，但很整洁）
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 添加 src 子目录，实际的构建定义在里面
add_subdirectory(src)