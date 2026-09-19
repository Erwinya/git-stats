# git-stats

C++17 CLI that summarizes a Git repository: commit count, authors, and recent subjects.

Uses the `git` CLI on PATH (no packfile parsing). Walks upward to find `.git`.

## Status

Library, CLI, and build scripts (`Makefile`, `build.bat`) are in place.

## Build

```bash
make
./git-stats --path . --recent 5
```

Windows (MinGW / LLVM):

```bat
build.bat
build\git-stats.exe --path . --recent 5
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
