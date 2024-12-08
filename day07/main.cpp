#include <ctype.h>
#include <stdio.h>

#include <iostream>
#include <parser/Tokenizer.hpp>
#include <string>
#include <util/FileUtil.hpp>
#include <util/StringUtil.hpp>
#include <vector>

using namespace cam::util;
using namespace cam::parser;

struct Test {
    int64_t value;
    std::vector<int64_t> operands;
};

enum class OperatorType { SUM, MULTIPLY, CONCAT };

std::vector<Test> parse_input(const std::vector<std::string> &lines) {
    std::vector<Test> ret;
    for (const std::string &line : lines) {
        Tokenizer tkn(line);
        Test item;
        item.value = tkn.consumeInteger();
        if (!tkn.advanceIfEqual(':')) {
            std::cerr << "[ERROR] Parse have no the correct format."
                      << std::endl;
        }
        tkn.consume();
        item.operands = tkn.consumeIntegerList(" ");
        ret.push_back(item);
    }

    return ret;
}

bool solve_recursive(const std::vector<int64_t> &operands, int32_t level, int64_t val,
           OperatorType op, int64_t check, bool isPart2) {
    if (level == operands.size()) {
        return (val == check);
    }

    int64_t tmp = 0;
    if(op == OperatorType::SUM) {
        tmp = val + operands[level];
    } else if(op == OperatorType::MULTIPLY) {
        tmp = val * operands[level];
    } else if(op == OperatorType::CONCAT) {
        std::string str = std::to_string(val) + std::to_string(operands[level]);
        tmp = std::stoll(str);
    }

    bool ret = false;
    ret |= solve_recursive(operands, level + 1, tmp, OperatorType::SUM, check, isPart2);
    ret |= solve_recursive(operands, level + 1, tmp, OperatorType::MULTIPLY, check, isPart2);
    if(isPart2) {
        ret |= solve_recursive(operands, level + 1, tmp, OperatorType::CONCAT, check, isPart2);
    }
    return ret;
}


size_t solve(const std::vector<Test> &tests, bool isPart2) {
    size_t ret = 0;
    for (const auto &test : tests) {
        if (solve_recursive(test.operands, 0, 0, OperatorType::SUM, test.value, isPart2)) {
            ret += test.value;
        }
    }

    return ret;
}

size_t part1(const std::vector<Test> &tests) {
    return solve(tests, false);
}

size_t part2(const std::vector<Test> &tests) {
    return solve(tests, true);
}

int main(int argc, char **argv) {
    std::string prg = argv[0];
    std::string asset =
        FileUtil::pathRemoveComponents(prg, 1) + "/assets/test01.txt";

    std::vector<uint8_t> fileContent = FileUtil::fileRead(asset);
    std::string content(fileContent.begin(), fileContent.end());
    if (content.empty()) {
        std::cerr << "[ERROR] Invalid input file: " << asset << std::endl;
        return -1;
    }

    std::vector<std::string> lines = StringUtil::split(content, "\n");
    std::vector<Test> tests = parse_input(lines);

    size_t p1 = part1(tests);
    std::cout << "Part 1 total: " << p1 << std::endl;

    size_t p2 = part2(tests);
    std::cout << "Part 2 total: " << p2 << std::endl;

    return 0;
}