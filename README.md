# git-stats

C++17 CLI that summarizes a Git repository: commit count, authors, and recent subjects.

Uses the `git` CLI on PATH (no packfile parsing). Walks upward to find `.git`.

## Status

Git root discovery, commit/author collection, and recent subjects are in place. CLI entrypoint and build scripts will land in follow-up commits.

## Library (so far)

```cpp
#include "git_stats.hpp"

auto root = gitstats::find_git_root(".");
auto stats = gitstats::collect(root, /*recent_limit=*/5);
// stats.commit_count, stats.commits_by_author, stats.recent_subjects
```

## Requirements

- C++17 compiler
- `git` available on PATH

## License

MIT
