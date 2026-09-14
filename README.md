# git-stats

C++17 CLI that summarizes a Git repository: commit count, authors, and recent subjects.

Uses the `git` CLI on PATH (no packfile parsing). Walks upward to find `.git`.

## Build

```bat
build.bat
```

## Usage

```bash
./git-stats --path ../some-repo --recent 5
```

## License

MIT
