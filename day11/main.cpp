#include <util/StringUtil.hpp>
#include <util/FileUtil.hpp>
#include <parser/Tokenizer.hpp>

#include <stdio.h>
#include <ctype.h>
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <cmath>

using namespace cam::util;
using namespace cam::parser;

using spplited = std::pair<int64_t, int64_t>;
using dict     = std::map<int64_t, std::vector<int64_t>>;

size_t
int_len(int64_t val) {
    size_t magnitude = 10;
    size_t len       = 1;

    while(true) {
        if(val < magnitude) {
            return len;
        }
        magnitude *= 10;
        len++;
    }
    return 0;
}

spplited
int_split(int64_t val) {
    size_t len       = int_len(val);
    size_t magnitude = std::pow(10, len / 2);

    int64_t high = val / magnitude;
    int64_t low  = val % magnitude;

    return spplited(high, low);
}

std::map<int64_t, int64_t>
iterate(std::map<int64_t, int64_t> stones) {
    std::map<int64_t, int64_t> next;
    for(auto [key, val] : stones) {
        if(key == 0) {
            next[1] += val;
        } else if(int_len(key) % 2 == 0) {
            spplited p = int_split(key);
            next[p.first] += val;
            next[p.second] += val;
        } else {
            next[key * 2024] += val;
        }
    }
    return next;
}

size_t
solve(std::vector<int64_t> stones, bool isPart2) {
    std::map<int64_t, int64_t> map;
    for(auto v : stones) {
        map[v] = 1;
    }

    size_t count = isPart2 ? 75 : 25;
    for(size_t i = 0; i < count; i++) {
        map = iterate(map);
    }

    size_t ret = 0;
    for(auto [key, val] : map) {
        ret += val;
    }
    return ret;
}

size_t
part1(const std::vector<int64_t> &stones) {
    return solve(stones, false);
}

size_t
part2(const std::vector<int64_t> &stones) {
    return solve(stones, true);
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

    Tokenizer            tkn(content);
    std::vector<int64_t> stones = tkn.consumeIntegerList();

    size_t p1 = part1(stones);
    std::cout << "Part 1 total: " << p1 << std::endl;

    size_t p2 = part2(stones);
    std::cout << "Part 2 total: " << p2 << std::endl;

    return 0;
}