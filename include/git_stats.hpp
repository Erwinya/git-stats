#pragma once

#include <map>
#include <string>

namespace gitstats {

struct RepoStats {
    std::string root;
    std::size_t commit_count = 0;
    std::map<std::string, std::size_t> commits_by_author;
};

/// Finds `.git` walking upward from `start_dir`. Empty string if not found.
std::string find_git_root(const std::string &start_dir);

/// Collect commit count and author totals via the `git` CLI (must be on PATH).
RepoStats collect(const std::string &repo_root);

}  // namespace gitstats
