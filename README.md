# 备份工具 (BackupTool)

#项目简介
这是一个C++编写的简易文件备份工具，支持文件压缩与还原功能。

#提交物说明
- `build/bin/Backuptool.exe`: 在MinGW-w64 (GCC)环境下编译的可执行程序。
- `include/`: 项目的所有头文件（.h），包含类与函数的声明。
- `src/`: 项目的全部源代码。
- `CMakeLists.txt`: 项目构建配置文件。
- `build.bat`: 一键自动构建脚本（针对Windows + MinGW环境）。
- `README.md`: 本说明文件。

# 运行说明
我们提供了自动化构建脚本 `build.bat`。
1.  环境准备：请确保您的Windows系统已安装：
      CMake (>= 3.10)
       MinGW-w64(GCC编译器)
      并将两者的 `bin` 目录添加到系统的 `PATH` 环境变量中。
2.  构建步骤：
    - 双击运行 `build.bat`，或手动执行以下命令：
      mkdir build
      cd build
      cmake .. -G “MinGW Makefiles”
      make
    - 构建成功后，新的可执行文件将生成在 `build/bin/` 目录下。

本项目源代码主要面向GCC编译环境，若使用Visual Studio (MSVC) 直接编译可能因语法细节差异而失败。

