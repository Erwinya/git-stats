#pragma once

#include <string>

namespace gitstats {

/// Finds `.git` walking upward from `start_dir`. Empty string if not found.
std::string find_git_root(const std::string &start_dir);

}  // namespace gitstats
