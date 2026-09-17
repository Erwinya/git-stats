#include "git_stats.hpp"

#include <array>
#include <cctype>
#include <cstdio>
#include <sstream>
#include <stdexcept>

namespace gitstats {
namespace {

std::string run_git(const std::string &repo, const std::string &args) {
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
    if (rc != 0) {
        throw std::runtime_error("git command failed (" + std::to_string(rc) + "): " + cmd);
    }
    return out;
}

}  // namespace

RepoStats collect(const std::string &repo_root) {
    RepoStats stats;
    stats.root = repo_root;

    std::string count_text = run_git(repo_root, "rev-list --count HEAD");
    stats.commit_count = static_cast<std::size_t>(std::stoull(count_text));

    std::string shortlog = run_git(repo_root, "shortlog -sn --all");
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
        stats.commits_by_author[author] += n;
    }
    return stats;
}

}  // namespace gitstats
