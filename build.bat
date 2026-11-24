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

if /I "%~1"=="test" (
  where py >nul 2>&1
  if not errorlevel 1 (
    py -3 tests\cli_tests.py
  ) else (
    python tests\cli_tests.py
  )
  if errorlevel 1 exit /b 1
) else (
  echo Example: build\git-stats.exe --path . --recent 5
  echo Test:    build.bat test
)
endlocal
