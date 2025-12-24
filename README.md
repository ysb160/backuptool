# 备份工具 (BackupTool)

## 项目简介
这是一个C++编写的简易文件备份工具，支持文件压缩与还原功能。

## 提交物说明
- `build/bin/Backuptool.exe`: **在MinGW-w64 (GCC)环境下编译的可执行程序**。
- `include/`: **项目的所有头文件（.h）**，包含类与函数的声明。
- `src/`: 项目的全部源代码。
- `CMakeLists.txt`: 项目构建配置文件。
- `build.bat`: 一键自动构建脚本（针对Windows + MinGW环境）。
- `README.md`: 本说明文件。

## 运行说明 (请助教优先阅读)

### 情况一：直接运行可执行文件 (快速验证)
双击 `bin/Backuptool.exe` 运行。
*   **注意**：此程序在 **MinGW-w64 (GCC)** 环境下编译。如果您的系统缺少必要的运行时库（如 `libgcc_s_seh-1.dll`, `libstdc++-6.dll`），程序将无法启动。
*   **若出现dll错误**：您可以尝试从MinGW-w64的`bin`目录拷贝缺失的dll到`Backuptool.exe`同目录，但我们更推荐您使用下面的“重新构建”方式。

### 情况二：在本地重新构建 (推荐，最可靠)
我们提供了自动化构建脚本 `build.bat`。
1.  **环境准备**：请确保您的Windows系统已安装：
    *   **CMake** (>= 3.10)
    *   **MinGW-w64** (GCC编译器)
    *   并将两者的 `bin` 目录添加到系统的 `PATH` 环境变量中。
2.  **构建步骤**：
    - 打开命令行。
    - 进入本项目根目录。
    - **双击运行 `build.bat`**，或手动执行以下命令：
      ```bash
      mkdir build
      cd build
      cmake .. -G “MinGW Makefiles”
      make
      ```
    - 构建成功后，新的可执行文件将生成在 `build/bin/` 目录下。

### 情况三：使用其他编译器 (如Visual Studio)
本项目源代码主要面向GCC编译环境。
*   使用Visual Studio (MSVC) 直接编译**可能因语法细节差异而失败**。
*   若需在VS中打开，请使用 **“打开 -> 文件夹”** 功能，打开本项目根目录，VS将自动识别 `CMakeLists.txt` 并配置。

## 项目结构