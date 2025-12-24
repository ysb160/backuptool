@echo off
echo ========================================
echo    BackupTool 项目自动构建脚本
echo ========================================
echo 此脚本使用 CMake 和 MinGW 构建项目。
echo.

REM 1. 检查必要命令是否存在
where cmake >nul 2>nul
if errorlevel 1 (
    echo [错误] 未找到 CMake。请安装CMake并确保其bin目录在PATH中。
    pause
    exit /b 1
)

REM 尝试查找 make 或 mingw32-make
where make >nul 2>nul && set MAKE_CMD=make
if not defined MAKE_CMD (
    where mingw32-make >nul 2>nul && set MAKE_CMD=mingw32-make
)
if not defined MAKE_CMD (
    echo [错误] 未找到 make 或 mingw32-make。请确保MinGW已正确安装并配置。
    pause
    exit /b 1
)

echo [信息] 使用构建工具：%MAKE_CMD%
echo.

REM 2. 清理并创建构建目录
if exist build (
    echo [信息] 清理旧的构建目录...
    rmdir /s /q build
)
mkdir build 2>nul
cd build

REM 3. 生成构建系统（关键：不硬编码-G，让CMake自动寻找MinGW）
echo [信息] 正在配置项目(CMake)...
cmake .. -G "MinGW Makefiles"
if errorlevel 1 (
    echo.
    echo [错误] CMake配置失败！请检查：
    echo       1. MinGW的g++.exe是否在PATH中。
    echo       2. 项目CMakeLists.txt是否有语法错误。
    cd ..
    pause
    exit /b 1
)

REM 4. 编译项目
echo.
echo [信息] 正在编译项目(%MAKE_CMD%)...
%MAKE_CMD%
if errorlevel 1 (
    echo.
    echo [错误] 编译失败！请检查源代码。
    cd ..
    pause
    exit /b 1
)

REM 5. 完成
echo.
echo ========================================
echo           构建成功！
echo ========================================
echo 生成的可执行文件在：%cd%\bin\Backuptool.exe
echo.
echo 您可以通过以下命令运行：
echo   .\build\bin\Backuptool.exe
echo.
cd ..
pause