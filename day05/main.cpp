#include <util/StringUtil.hpp>
#include <util/FileUtil.hpp>
#include <parser/Tokenizer.hpp>

#include <stdio.h>
#include <ctype.h>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>

using namespace cam::util;
using namespace cam::parser;

struct Rule {
    int first;
    int second;
};

struct Input {
    std::map<int32_t, std::vector<int32_t>> rules;
    std::vector<std::vector<int32_t>>       data;
};

Input
parseInput(const std::string &content) {
    Input                    ret;
    std::vector<std::string> blocks    = StringUtil::split(content, "\n\n");
    std::vector<std::string> ruleLines = StringUtil::split(blocks[0], "\n");
    std::vector<std::string> pageLines = StringUtil::split(blocks[1], "\n");

    // build rules map
    for(const auto &line : ruleLines) {
        Tokenizer tkn(line);
        int32_t   f = tkn.consumeInteger();
        if(!tkn.advanceIfEqual('|')) {
            std::cerr << "[ERROR] Parse error! '" << line << "' doesn't match the pattern! (key: " << f << ")" << std::endl;
        }
        tkn.consume();
        int32_t s = tkn.consumeInteger();
        if(s <= 0) {
            std::cerr << "[ERROR] Parse error! '" << line << "' doesn't match the pattern! (key: " << f << ", val: " << s << ")"
                      << std::endl;
        }
        ret.rules[f].push_back(s);
    }

    // build page matrix
    for(const auto &line : pageLines) {
        Tokenizer tkn(line);
        ret.data.push_back(tkn.consumeIntegerList(","));
    }

    return ret;
}

size_t
solve(std::vector<int32_t> line, const Input &inputs, bool isPart2) {
    size_t ret     = 0;
    bool   matches = true;
    bool   fixed   = false;
    bool   isValid = !isPart2;

    for(size_t i = 0; i < line.size(); i++) {
        int32_t key = line[i];
        if(inputs.rules.find(key) == inputs.rules.end()) {
            continue;
        }

        auto rules = inputs.rules.at(key);
        for(size_t j = 0; j < i; j++) {
            int32_t current = line[j];
            if(std::find(rules.begin(), rules.end(), current) != rules.end()) {
                if(isPart2) {
                    line[i] = current;
                    line[j] = key;
                    i--;
                    isValid = true;
                } else {
                    isValid = false;
                }
                break;
            }
        }

        if(!isPart2 && !isValid) {
            break;
        }
    }

    if(isValid) {
        ret = line[line.size() / 2];
    }
    return ret;
}

size_t
part1(const Input &inputs) {
    size_t ret = 0;
    for(const auto &line : inputs.data) {
        ret += solve(line, inputs, false);
    }
    return ret;
}

size_t
part2(const Input &inputs) {
    size_t ret = 0;
    for(auto line : inputs.data) {
        ret += solve(line, inputs, true);
    }
    return ret;
}

int
main(int argc, char **argv) {
    std::string prg   = argv[0];
    std::string asset = FileUtil::pathRemoveComponents(prg, 1) + "/assets/input01.txt";

    std::vector<uint8_t> fileContent = FileUtil::fileRead(asset);
    std::string          content(fileContent.begin(), fileContent.end());
    if(content.empty()) {
        std::cout << "[ERROR] Invalid input file: " << asset << std::endl;
        return -1;
    }

    Input inputs = parseInput(content);

    size_t p1 = part1(inputs);
    std::cout << "Part 1 total: " << p1 << std::endl;

    size_t p2 = part2(inputs);
    std::cout << "Part 2 total: " << p2 << std::endl;

    return 0;
}
