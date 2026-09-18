#pragma once

#include <map>
#include <string>
#include <vector>

namespace gitstats {

struct RepoStats {
    std::string root;
    std::size_t commit_count = 0;
    std::map<std::string, std::size_t> commits_by_author;
    std::vector<std::string> recent_subjects;
};

/// Finds `.git` walking upward from `start_dir`. Empty string if not found.
std::string find_git_root(const std::string &start_dir);

/// Collect commit count, author totals, and recent subjects via the `git` CLI.
RepoStats collect(const std::string &repo_root, std::size_t recent_limit = 10);

/// Print a human-readable summary to stdout.
void print_report(const RepoStats &stats);

}  // namespace gitstats
