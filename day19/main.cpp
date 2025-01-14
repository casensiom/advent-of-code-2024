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
solve(const std::vector<std::string> &keys, const std::string &target, const std::string &solution, std::vector<std::string> &visited) {
    if(solution.size() >= target.size()) {
        return 0;
    }

    size_t ret = 0;
    for(const auto &piece : keys) {
        std::string next = solution + piece;
        if(std::find(visited.begin(), visited.end(), next) != visited.end()) {
            continue;
        }
        if(StringUtil::starts(target, next)) {
            visited.push_back(next);
            // std::cout << "trace: " << solution << " + " << piece << std::endl;
            if(next == target) {
                return 1;
            } else {
                ret += solve(keys, target, next, visited);
            }
        }
    }
    return ret;
}

size_t
part1(const std::vector<std::string> &values, const std::vector<std::string> &targets) {
    size_t ret = 0;

    for(const auto t : targets) {
        std::vector<std::string> visited;
        ret += solve(values, t, "", visited);
        std::cout << " > " << t << std::endl;
    }
    return ret;
}

size_t
part2(const std::vector<std::string> &lines) {
    return 0;
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

    std::sort(dict.begin(), dict.end(), [](const std::string &a, const std::string &b) { return a.size() > b.size(); });
    size_t p1 = part1(dict, targets);
    std::cout << "Part 1 total: " << p1 << std::endl;

    // size_t p2 = part2(lines);
    // std::cout << "Part 2 total: " << p2 << std::endl;

    return 0;
}