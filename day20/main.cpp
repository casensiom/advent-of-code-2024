#include <util/StringUtil.hpp>
#include <util/FileUtil.hpp>
#include <parser/Tokenizer.hpp>
#include <pathfinder/PathFinder.hpp>
#include <math/Vector2.hpp>

#include <stdio.h>
#include <ctype.h>
#include <string>
#include <vector>
#include <iostream>

using namespace cam::util;
using namespace cam::parser;
using namespace cam::pathfinder;
using namespace cam::math;

//              2
//             212
//            21x12
//             212
//              2

//
//              x12345678901234567890
//              12345678901234567890
//              2345678901234567890
//              345678901234567890
//              45678901234567890
//              5678901234567890
//              678901234567890
//              78901234567890
//              8901234567890
//              901234567890
//              01234567890
//              1234567890
//              234567890
//              34567890
//              4567890
//              567890
//              67890
//              7890
//              890
//              90
//              0

size_t
get_map(const std::vector<std::vector<int>> &map, Vector2i pos) {
    const int    x      = pos.getX();
    const int    y      = pos.getY();
    const size_t HEIGHT = map.size();
    const size_t WIDTH  = map[0].size();

    size_t ret = 0;
    if(x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        ret = map[y][x];
    }
    return ret;
}

size_t
improve(const std::vector<std::vector<int>> &map, size_t x, size_t y, size_t len, size_t limit) {
    const size_t HEIGHT = map.size();
    const size_t WIDTH  = map[0].size();

    Vector2i pos(x, y);
    size_t   ret     = 0;
    int      current = get_map(map, pos);
    int      ilen    = (int)len;
    for(int y = -ilen; y <= ilen; y++) {
        for(int x = -ilen; x <= ilen; x++) {
            int jump = abs(x) + abs(y);
            if(jump > ilen) {
                continue;
            }
            Vector2i npos = pos + Vector2i(x, y);
            int      next = get_map(map, npos);
            int      diff = (next - current) - jump;
            if(next == 0 || next < current || diff <= 0) {
                continue;
            }

            if(diff >= limit) {
                ret++;
            }
        }
    }
    return ret;
}

size_t
solve(const std::vector<std::string> &lines, size_t len, size_t limit) {
    PathFinder pf;

    const size_t HEIGHT = lines.size();
    const size_t WIDTH  = lines[0].size();

    pf.set(lines);
    auto solv = pf.solve();
    // pf.dump();

    std::vector<std::vector<int>> map(HEIGHT, std::vector<int>(WIDTH, 0));
    for(size_t i = 0; i < solv.size(); i++) {
        int x     = solv[i].getX();
        int y     = solv[i].getY();
        map[y][x] = i + 1;
    }

    size_t ret = 0;
    for(size_t i = 0; i < solv.size(); i++) {
        int x = solv[i].getX();
        int y = solv[i].getY();
        ret += improve(map, x, y, len, limit);
    }

    return ret;
}

size_t
part1(const std::vector<std::string> &lines) {
    return solve(lines, 2, 100);
}

size_t
part2(const std::vector<std::string> &lines) {
    return solve(lines, 20, 100);
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

    std::vector<std::string> lines = StringUtil::split(content, "\n");

    size_t p1 = part1(lines);
    std::cout << "Part 1 total: " << p1 << std::endl;

    size_t p2 = part2(lines);
    std::cout << "Part 2 total: " << p2 << std::endl;

    return 0;
}