#include <util/StringUtil.hpp>
#include <util/FileUtil.hpp>
#include <parser/Tokenizer.hpp>

#include <stdio.h>
#include <ctype.h>
#include <string>
#include <vector>
#include <map>
#include <iostream>

using namespace cam::util;
using namespace cam::parser;

size_t
solve_recursive(const std::vector<std::string> &keys, const std::string &target, const std::string &solution,
                std::map<std::string, size_t> &visited) {
    if(solution == target) {
        // std::cout << "found: " << solution << std::endl;
        return 1;
    }
    if(visited.find(solution) != visited.end()) {
        // std::cout << "retry: " << solution << std::endl;
        return visited[solution];
    }

    size_t ret = 0;
    for(const auto &piece : keys) {
        std::string next    = solution + piece;
        size_t      partial = 0;
        if(StringUtil::starts(target, next)) {
            // std::cout << "trace: " << solution << " + " << piece << std::endl;
            partial = solve_recursive(keys, target, next, visited);
        }
        visited[next] = partial;
        ret += partial;
    }
    visited[solution] = ret;
    return ret;
}

size_t
solve(const std::vector<std::string> &values, const std::vector<std::string> &targets) {
    size_t ret1 = 0;
    size_t ret2 = 0;

    for(const auto t : targets) {
        std::map<std::string, size_t> visited;
        size_t                        v = solve_recursive(values, t, "", visited);
        std::cout << " > " << t << " - " << v << std::endl;
        if(v > 0) {
            ret1++;
        }
        ret2 += v;
    }
    std::cout << "Part 1 total: " << ret1 << std::endl;
    std::cout << "Part 2 total: " << ret2 << std::endl;

    return ret1;
}

int
main(int argc, char **argv) {
    std::string prg   = argv[0];
    std::string asset = FileUtil::pathRemoveComponents(prg, 1) + "/assets/input01.txt";

    std::vector<uint8_t> fileContent = FileUtil::fileRead(asset);
    std::string          content(fileContent.begin(), fileContent.end());
    if(content.empty()) {
        std::cerr << "[ERROR] Invalid input file: " << asset << std::endl;
        return -1;
    }

    std::vector<std::string> parts = StringUtil::split(content, "\n\n");
    Tokenizer                tkn;

    tkn.set(parts[0]);
    std::vector<std::string> dict    = tkn.consumeStringList(", ");
    std::vector<std::string> targets = StringUtil::split(parts[1], "\n");

    std::sort(dict.begin(), dict.end(), [](const std::string &a, const std::string &b) { return a.size() < b.size(); });
    solve(dict, targets);

    return 0;
}