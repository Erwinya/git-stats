# git-stats

C++17 CLI that summarizes a Git repository: commit count, authors, and recent subjects.

Uses the `git` CLI on PATH (no packfile parsing). Walks upward to find `.git`.

## Status

Library, CLI, automated CLI tests, and CI (`Makefile`, `build.bat`, GitHub Actions).

## Build

```bash
make
./git-stats --path . --recent 5
make test
```

Windows (MinGW / LLVM):

```bat
build.bat
build\git-stats.exe --path . --recent 5
build.bat test
```

## Behavior

- `--path` / `-C` choose the starting directory for `.git` discovery.
- `--recent N` must be a non-negative integer; invalid usage exits with code `2`.
- Commit count, author totals, and recent subjects all use `HEAD` (not mixed with `--all`).
- Authors are ranked by commit count (descending), then name.
- Empty repositories report `0` commits without failing.

Exit codes: `0` ok, `1` runtime error, `2` bad usage.

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
- Python 3 for `tests/cli_tests.py`

## License

MIT
