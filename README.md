# git-stats

C++17 CLI that summarizes a Git repository: commit count, authors, and recent subjects.

Uses the `git` CLI on PATH (no packfile parsing). Walks upward to find `.git`.

## Status

Library APIs plus a CLI entrypoint (`src/main.cpp`) are in place. Makefile / `build.bat` will land in a follow-up commit.

## Build (manual)

```powershell
g++ -std=c++17 -I include -o git-stats.exe src\git_root.cpp src\git_stats.cpp src\main.cpp
.\git-stats.exe --path . --recent 5
```

## Library

```cpp
#include "git_stats.hpp"

auto root = gitstats::find_git_root(".");
auto stats = gitstats::collect(root, /*recent_limit=*/5);
gitstats::print_report(stats);
```

## Requirements

- C++17 compiler
- `git` available on PATH

## License

MIT
