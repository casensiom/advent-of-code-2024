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

int32_t
get_span(const std::vector<int32_t> &disk, int32_t limit, int32_t len) {
    int32_t pos = -1;

    int32_t idxLeft = 0;
    do {
        while(disk[idxLeft] != -1) {
            idxLeft++;
        }
        int32_t lLen = 0;
        while(disk[idxLeft + lLen] == -1) {
            lLen++;
        }
        if(lLen >= len && idxLeft < limit) {
            pos = idxLeft;
            break;
        } else {
            idxLeft += lLen;
        }
    } while(idxLeft < limit);
    return pos;
}

std::vector<int32_t>
get_disk(const std::string &line) {
    std::vector<int32_t> disk;
    int32_t              even = true;
    int32_t              id   = 0;
    for(unsigned char c : line) {
        int32_t len = c - '0';
        int32_t val = (even ? id : -1);
        for(size_t i = 0; i < len; i++) {
            disk.push_back(val);
        }
        id += (even ? 1 : 0);
        even = !even;
    }
    return disk;
}

size_t
score(const std::vector<int32_t> &disk) {
    size_t ret = 0;
    for(size_t i = 0; i < disk.size(); i++) {
        if(disk[i] == -1) {
            continue;
        }
        ret += i * disk[i];
    }
    return ret;
}

size_t
solve(const std::string &line) {
    std::vector<int32_t> disk = get_disk(line);

    int32_t idxLeft  = 0;
    int32_t idxRight = disk.size() - 1;
    while(idxLeft < idxRight) {
        while(disk[idxLeft] != -1) {
            idxLeft++;
        }
        while(disk[idxRight] == -1) {
            idxRight--;
        }

        disk[idxLeft]  = disk[idxRight];
        disk[idxRight] = -1;

        idxLeft++;
        idxRight--;
    }

    return score(disk);
}

size_t
solve2(const std::string &line) {
    std::vector<int32_t> disk = get_disk(line);

    int32_t idxLeft  = 0;
    int32_t idxRight = disk.size() - 1;
    while(idxLeft < idxRight) {
        while(disk[idxRight] == -1) {
            idxRight--;
        }
        int32_t val  = disk[idxRight];
        int32_t rLen = 0;
        while(disk[idxRight - rLen] == val) {
            rLen++;
        }

        int32_t pos = get_span(disk, idxRight, rLen);
        if(pos != -1) {
            for(size_t i = 0; i < rLen; i++) {
                disk[pos + i]      = val;
                disk[idxRight - i] = -1;
            }
        }
        idxRight -= rLen;
    }

    return score(disk);
}

size_t
part1(const std::string &line) {
    return solve(line);
}

size_t
part2(const std::string &line) {
    return solve2(line);
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

    size_t p2 = part2(content);
    std::cout << "Part 2 total: " << p2 << std::endl;

    return 0;
}