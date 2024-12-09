#include <util/StringUtil.hpp>
#include <util/FileUtil.hpp>
#include <parser/Tokenizer.hpp>

#include <stdio.h>
#include <ctype.h>
#include <string>
#include <vector>
#include <iostream>

using namespace cam::util;
using namespace cam::parser;

size_t
solve(const std::string &line, bool isPart2) {
    size_t ret = 0;

    // abababababab
    // ^          ^
    // left       right

    std::cout << line << std::endl;

    size_t maxId    = std::ceil(line.size() / 2);
    size_t skipLast = (line.size() % 2) == 0;

    size_t left  = 0;
    size_t right = line.size() - (((line.size() % 2) == 0) ? 2 : 1);

    size_t index     = 0;
    size_t currentId = 0;
    size_t lastId    = maxId;
    size_t count2    = 0;
    while(left <= (right + 1)) {
        // consume a valid value
        size_t count = line[left++] - '0';
        for(size_t i = 0; i < count; i++) {
            ret += index * currentId;
            std::cout << currentId << " ";
            index++;
        }
        currentId++;

        // consume a valid space
        size_t spaces = line[left++] - '0';
        if(spaces == 0) {
            continue;
        }

        // consume a compressed value until it ends or run out of space
        do {
            if(count2 == 0) {
                count2 = line[right] - '0';
                right -= 2;
            }
            while(count2 > 0 && spaces > 0) {
                ret += index * lastId;
                std::cout << lastId << " ";
                count2--;
                spaces--;
                index++;
            }
            if(count2 == 0) {
                lastId--;
            }
        } while(spaces > 0);
    }

    while(count2 > 0) {
        ret += index * lastId;
        std::cout << lastId << " ";
        count2--;
        index++;
    }

    std::cout << std::endl;
    return ret;
}

size_t
part1(const std::string &line) {
    return solve(line, false);
}

size_t
part2(const std::string &line) {
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

    size_t p1 = part1(content);
    std::cout << "Part 1 total: " << p1 << std::endl;
    // too high 6284186731475

    size_t p2 = part2(content);
    std::cout << "Part 2 total: " << p2 << std::endl;

    return 0;
}