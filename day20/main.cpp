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
improve(const std::vector<std::vector<int>> &map, size_t x, size_t y, size_t d) {
    std::vector<Vector2i> dirs = {
        { 0, -1},
        { 1,  0},
        { 0,  1},
        {-1,  0}
    };

    Vector2i dir = dirs[d];
    Vector2i pos(x, y);

    int current = get_map(map, pos);
    int wall    = get_map(map, pos + dir);
    int next    = get_map(map, pos + dir * 2);

    size_t ret = 0;
    if(current != 0 && wall == 0 && next != 0 && next > current) {
        ret = (next - current) - 2;
    }

    return ret;
}

size_t
solve(const std::vector<std::string> &lines, size_t limit) {
    size_t     ret = 0;
    PathFinder pf;

    const size_t HEIGHT = lines.size();
    const size_t WIDTH  = lines[0].size();

    pf.set(lines);
    auto solv = pf.solve();
    pf.dump();

    std::vector<std::vector<int>> map(HEIGHT, std::vector<int>(WIDTH, 0));
    for(size_t i = 0; i < solv.size(); i++) {
        int x     = solv[i].getX();
        int y     = solv[i].getY();
        map[y][x] = i + 1;
    }

    for(size_t i = 0; i < solv.size(); i++) {
        int x = solv[i].getX();
        int y = solv[i].getY();
        for(size_t d = 0; d < 4; d++) {
            size_t diff = improve(map, x, y, d);
            if(diff >= limit) {
                std::cout << "break: (" << x << ", " << y << ") [" << d << "] = " << diff << std::endl;
                ret++;
            }
        }
    }

    return ret;
}

size_t
part1(const std::vector<std::string> &lines) {
    return solve(lines, 100);
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

    std::vector<std::string> lines = StringUtil::split(content, "\n");

    size_t p1 = part1(lines);
    std::cout << "Part 1 total: " << p1 << std::endl;

    size_t p2 = part2(lines);
    std::cout << "Part 2 total: " << p2 << std::endl;

    return 0;
}