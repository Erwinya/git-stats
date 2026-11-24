#include "git_stats.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

namespace {

std::size_t parse_recent(const char *text) {
    if (text == nullptr || *text == '\0') {
        throw std::invalid_argument("--recent requires a non-negative integer");
    }
    char *end = nullptr;
    unsigned long value = std::strtoul(text, &end, 10);
    if (end == text || *end != '\0') {
        throw std::invalid_argument("--recent must be a non-negative integer");
    }
    // Reject values that look negative when passed through argv (e.g. "-1").
    if (text[0] == '-') {
        throw std::invalid_argument("--recent must be a non-negative integer");
    }
    return static_cast<std::size_t>(value);
}

}  // namespace

int main(int argc, char **argv) {
    std::string start = ".";
    std::size_t recent = 10;
    try {
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            if ((arg == "--path" || arg == "-C") && i + 1 < argc) {
                start = argv[++i];
            } else if (arg == "--path" || arg == "-C") {
                throw std::invalid_argument(arg + " requires a directory");
            } else if (arg == "--recent" && i + 1 < argc) {
                recent = parse_recent(argv[++i]);
            } else if (arg == "--recent") {
                throw std::invalid_argument("--recent requires a non-negative integer");
            } else if (arg == "--help" || arg == "-h") {
                std::cout << "Usage: git-stats [--path DIR] [--recent N]\n"
                          << "Summarize commit/author stats for a Git repository.\n"
                          << "Exit codes: 0 ok, 1 runtime error, 2 bad usage.\n";
                return 0;
            } else {
                std::cerr << "unknown argument: " << arg << '\n';
                return 2;
            }
        }
    } catch (const std::exception &ex) {
        std::cerr << "error: " << ex.what() << '\n';
        return 2;
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
