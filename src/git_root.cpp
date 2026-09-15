#include "git_stats.hpp"

#include <filesystem>

namespace fs = std::filesystem;

namespace gitstats {

std::string find_git_root(const std::string &start_dir) {
    fs::path p = fs::absolute(start_dir);
    while (true) {
        if (fs::exists(p / ".git")) {
            return p.string();
        }
        if (!p.has_parent_path() || p == p.root_path()) {
            return {};
        }
        p = p.parent_path();
    }
}

}  // namespace gitstats
