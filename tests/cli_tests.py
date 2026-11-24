#!/usr/bin/env python3
"""CLI smoke tests for git-stats (stdlib only)."""
from __future__ import annotations

import os
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


def find_exe() -> Path:
    candidates = [
        ROOT / "build" / "git-stats.exe",
        ROOT / "git-stats.exe",
        ROOT / "git-stats",
    ]
    for path in candidates:
        if path.is_file():
            return path
    raise SystemExit("git-stats binary not found; run build.bat or make first")


def run(exe: Path, *args: str, cwd: Path | None = None) -> subprocess.CompletedProcess[str]:
    return subprocess.run(
        [str(exe), *args],
        cwd=cwd or ROOT,
        text=True,
        capture_output=True,
        check=False,
    )


def git(cwd: Path, *args: str) -> None:
    subprocess.run(["git", *args], cwd=cwd, check=True, capture_output=True, text=True)


def test_help(exe: Path) -> None:
    proc = run(exe, "--help")
    assert proc.returncode == 0, proc.stderr
    assert "Usage:" in proc.stdout


def test_bad_recent(exe: Path) -> None:
    for args in (["--recent"], ["--recent", "-1"], ["--recent", "abc"]):
        proc = run(exe, *args)
        assert proc.returncode == 2, (args, proc.returncode, proc.stderr)
        assert "error:" in proc.stderr


def test_missing_repo(exe: Path) -> None:
    with tempfile.TemporaryDirectory() as tmp:
        proc = run(exe, "--path", tmp)
        assert proc.returncode == 1
        assert "no .git directory found" in proc.stderr


def test_empty_and_populated_repo(exe: Path) -> None:
    with tempfile.TemporaryDirectory() as tmp:
        repo = Path(tmp)
        git(repo, "init")
        git(repo, "config", "user.name", "Tester")
        git(repo, "config", "user.email", "tester@example.com")

        empty = run(exe, "--path", str(repo), "--recent", "3")
        assert empty.returncode == 0, empty.stderr
        assert "commits       : 0" in empty.stdout
        assert "(none)" in empty.stdout

        (repo / "README.md").write_text("hello\n", encoding="utf-8")
        git(repo, "add", "README.md")
        git(repo, "commit", "-m", "first commit")
        (repo / "NOTES.md").write_text("notes\n", encoding="utf-8")
        git(repo, "add", "NOTES.md")
        git(repo, "commit", "-m", "second commit")

        filled = run(exe, "--path", str(repo), "--recent", "5")
        assert filled.returncode == 0, filled.stderr
        assert "commits       : 2" in filled.stdout
        assert "Tester" in filled.stdout
        assert "first commit" in filled.stdout or "second commit" in filled.stdout


def main() -> int:
    exe = find_exe()
    tests = [
        test_help,
        test_bad_recent,
        test_missing_repo,
        test_empty_and_populated_repo,
    ]
    failed = 0
    for test in tests:
        try:
            test(exe)
            print(f"ok  {test.__name__}")
        except Exception as ex:  # noqa: BLE001
            failed += 1
            print(f"FAIL {test.__name__}: {ex}", file=sys.stderr)
    if failed:
        print(f"{failed} failed", file=sys.stderr)
        return 1
    print(f"{len(tests)} passed")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
