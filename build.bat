@echo off
setlocal
where g++ >nul 2>&1
if errorlevel 1 (
  echo g++ not found. Install LLVM-MinGW or MSYS2, then re-run build.bat
  exit /b 1
)
if not exist build mkdir build
g++ -std=c++17 -Wall -Wextra -Wpedantic -O2 -Iinclude src\main.cpp src\git_root.cpp src\git_stats.cpp -o build\git-stats.exe
if errorlevel 1 exit /b 1
echo Built build\git-stats.exe
echo Example: build\git-stats.exe --path . --recent 5
endlocal
