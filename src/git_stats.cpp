#include "git_stats.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <cstdio>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>

namespace gitstats {
namespace {

std::string run_git(const std::string &repo, const std::string &args, bool allow_fail = false) {
    std::string cmd = "git -C \"" + repo + "\" " + args;
#ifdef _WIN32
    FILE *pipe = _popen(cmd.c_str(), "r");
#else
    FILE *pipe = popen(cmd.c_str(), "r");
#endif
    if (!pipe) {
        throw std::runtime_error("failed to run: " + cmd);
    }
    std::string out;
    std::array<char, 512> buf{};
    while (fgets(buf.data(), static_cast<int>(buf.size()), pipe) != nullptr) {
        out += buf.data();
    }
#ifdef _WIN32
    int rc = _pclose(pipe);
#else
    int rc = pclose(pipe);
#endif
    if (rc != 0 && !allow_fail) {
        throw std::runtime_error("git command failed (" + std::to_string(rc) + "): " + cmd);
    }
    if (rc != 0) {
        return {};
    }
    return out;
}

std::string trim_trailing_newline(std::string s) {
    while (!s.empty() && (s.back() == '\n' || s.back() == '\r')) {
        s.pop_back();
    }
    return s;
}

}  // namespace

RepoStats collect(const std::string &repo_root, std::size_t recent_limit) {
    RepoStats stats;
    stats.root = repo_root;

    std::string count_text = trim_trailing_newline(run_git(repo_root, "rev-list --count HEAD", true));
    if (!count_text.empty()) {
        stats.commit_count = static_cast<std::size_t>(std::stoull(count_text));
    }

    // Same revision scope as commit count / recent subjects.
    std::string shortlog = run_git(repo_root, "shortlog -sn HEAD", true);
    std::map<std::string, std::size_t> by_author;
    std::istringstream in(shortlog);
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::size_t i = 0;
        while (i < line.size() && (line[i] == ' ' || line[i] == '\t')) ++i;
        std::size_t j = i;
        while (j < line.size() && std::isdigit(static_cast<unsigned char>(line[j]))) ++j;
        if (i == j) continue;
        std::size_t n = static_cast<std::size_t>(std::stoull(line.substr(i, j - i)));
        while (j < line.size() && (line[j] == ' ' || line[j] == '\t')) ++j;
        std::string author = line.substr(j);
        by_author[author] += n;
    }

    stats.commits_by_author.assign(by_author.begin(), by_author.end());
    std::sort(
        stats.commits_by_author.begin(),
        stats.commits_by_author.end(),
        [](const auto &a, const auto &b) {
            if (a.second != b.second) {
                return a.second > b.second;
            }
            return a.first < b.first;
        });

    if (recent_limit > 0 && stats.commit_count > 0) {
        std::string log_out = run_git(
            repo_root,
            "log -n " + std::to_string(recent_limit) + " --pretty=format:%s",
            true);
        std::istringstream log_in(log_out);
        while (std::getline(log_in, line)) {
            line = trim_trailing_newline(std::move(line));
            if (!line.empty()) {
                stats.recent_subjects.push_back(std::move(line));
            }
        }
    }
    return stats;
}

void print_report(const RepoStats &stats) {
    std::cout << "root          : " << stats.root << '\n';
    std::cout << "commits       : " << stats.commit_count << '\n';
    std::cout << "authors:\n";
    if (stats.commits_by_author.empty()) {
        std::cout << "  (none)\n";
    } else {
        for (const auto &entry : stats.commits_by_author) {
            std::cout << "  " << entry.second << "  " << entry.first << '\n';
        }
    }
    std::cout << "recent:\n";
    if (stats.recent_subjects.empty()) {
        std::cout << "  (none)\n";
    } else {
        for (const auto &subject : stats.recent_subjects) {
            std::cout << "  - " << subject << '\n';
        }
    }
}

}  // namespace gitstats
