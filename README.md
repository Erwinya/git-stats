# git-stats

C++17 CLI that summarizes a Git repository: commit count, authors, and recent subjects.

Uses the `git` CLI on PATH (no packfile parsing). Walks upward to find `.git`.

## Status

Git root discovery plus commit/author collection are in place. Recent subjects, CLI entrypoint, and build scripts will land in follow-up commits.

## Library (so far)

```cpp
#include "git_stats.hpp"

auto root = gitstats::find_git_root(".");
auto stats = gitstats::collect(root);
// stats.commit_count, stats.commits_by_author
```

## Requirements

- C++17 compiler
- `git` available on PATH

## License

MIT
