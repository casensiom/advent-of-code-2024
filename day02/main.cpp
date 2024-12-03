#include <util/StringUtil.hpp>
#include <util/FileUtil.hpp>
#include <parser/Tokenizer.hpp>

#include <stdio.h>
#include <ctype.h>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace cam::util;
using namespace cam::parser;

bool
valid_line(const std::vector<size_t> &copy) {
    int invalid = 0;
    int prev    = copy[0];
    for (size_t i = 1; i < copy.size(); i++) {
        int next = copy[i];
        int diff = next - prev;
        if(diff < 0 || diff == 0 || diff > 3) {
            invalid++;
            break;
        }
        prev = next; 
    }
    return (invalid == 0);
}


size_t
solve(const std::vector<std::vector<size_t>> &matrix, bool isSecondPart) {
    size_t safe = 0;
    for (const auto &line: matrix) {
        std::vector<size_t> copy = line;
        if(line.front() > line.back()) {
            std::reverse(copy.begin(), copy.end());    // always use ascendent vectors
        }

        if(valid_line(copy)) {
            safe++;
        } else if(isSecondPart) {
            for (size_t i = 0; i < copy.size(); i++) {
                std::vector<size_t> modif = copy;
                modif.erase(modif.begin() + i);
                if(valid_line(modif)) {
                    safe++;
                    break;
                }
            }
        }
    }   
    
    return safe;
}


size_t
part1(const std::vector<std::vector<size_t>> &matrix) {
    return solve(matrix, false);
}


size_t
part2(const std::vector<std::vector<size_t>> &matrix) {
    return solve(matrix, true);
}

int
main(int argc, char **argv) {
    std::vector<uint8_t>     fileContent = FileUtil::fileRead("assets/input01.txt");
    std::string              content(fileContent.begin(), fileContent.end());
    std::vector<std::string> lines = StringUtil::split(content, "\n");

    Tokenizer tokenizer;
    std::vector<std::vector<size_t>> matrix;
    for (const std::string &line : lines) {
        tokenizer.set(line);
        matrix.push_back(tokenizer.consumeIntegerList());
    }

    size_t p1 = part1(matrix);
    printf(" Part 1 total:  %lu\n", p1);

    size_t p2 = part2(matrix);
    printf(" Part 2 total:  %lu\n", p2);

    return 0;
}