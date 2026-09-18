#include "git_stats.hpp"

#include <iostream>
#include <string>

int main(int argc, char **argv) {
    std::string start = ".";
    std::size_t recent = 10;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if ((arg == "--path" || arg == "-C") && i + 1 < argc) {
            start = argv[++i];
        } else if (arg == "--recent" && i + 1 < argc) {
            recent = static_cast<std::size_t>(std::stoul(argv[++i]));
        } else if (arg == "--help" || arg == "-h") {
            std::cout << "Usage: git-stats [--path DIR] [--recent N]\n"
                      << "Summarize commit/author stats for a Git repository.\n";
            return 0;
        } else {
            std::cerr << "unknown argument: " << arg << '\n';
            return 2;
        }
    }

    try {
        std::string root = gitstats::find_git_root(start);
        if (root.empty()) {
            std::cerr << "no .git directory found from: " << start << '\n';
            return 1;
        }
        gitstats::RepoStats stats = gitstats::collect(root, recent);
        gitstats::print_report(stats);
        return 0;
    } catch (const std::exception &ex) {
        std::cerr << "error: " << ex.what() << '\n';
        return 1;
    }
}
