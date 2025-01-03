#include <util/StringUtil.hpp>
#include <util/FileUtil.hpp>
#include <parser/Tokenizer.hpp>
#include <math/Vector2.hpp>

#include <stdio.h>
#include <ctype.h>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <limits>

using namespace cam::util;
using namespace cam::parser;
using namespace cam::math;

struct Region {
    size_t area      = 0;
    size_t perimeter = 0;
    size_t corners   = 0;
};

unsigned char
get(const std::vector<std::string> &map, int x, int y) {
    const size_t HEIGHT = map.size();
    const size_t WIDTH  = map[0].size();
    if(x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        return map[y][x];
    }
    return '#';
}

Region
floodfill(const std::vector<std::string> &map, std::vector<std::vector<int>> &visited, const Vector2i &current) {
    const size_t HEIGHT = map.size();
    const size_t WIDTH  = map[0].size();
    Region       ret;

    if(visited[current.getY()][current.getX()]) {
        return {0, 0};
    }

    unsigned char val = get(map, current.getX(), current.getY());

    std::vector<Vector2i> queue;
    queue.push_back(current);
    while(!queue.empty()) {
        Vector2i pos = queue.at(0);
        queue.erase(queue.begin());

        if(visited[pos.getY()][pos.getX()]) {
            continue;
        }
        visited[pos.getY()][pos.getX()] = 1;

        for(int dy = -1; dy <= 1; dy++) {
            for(int dx = -1; dx <= 1; dx++) {
                Vector2i      nextPos = {pos.getX() + dx, pos.getY() + dy};
                unsigned char nextVal = get(map, nextPos.getX(), nextPos.getY());
                int           corner  = 0;

                if(dx == 0 && dy == 0) {    // center
                    ret.area++;
                    continue;
                } else if(dx == 0 || dy == 0) {    // axis aligned
                    if(nextVal != val) {
                        ret.perimeter++;
                    }
                } else {    // diagonals
                    unsigned char diagonal   = nextVal;
                    unsigned char vertical   = get(map, pos.getX(), pos.getY() + dy);
                    unsigned char horizontal = get(map, pos.getX() + dx, pos.getY());
                    if(vertical == val && horizontal == val && diagonal != val) {
                        ret.corners++;
                    }

                    // the value in the diagonal can be equal to val, if the sides are not equal is enough
                    if(vertical != val && horizontal != val) {
                        ret.corners++;
                    }

                    continue;
                }

                if(nextVal == val) {
                    queue.push_back(nextPos);
                }
            }
        }
    }

    // std::cout << "Process val: " << val << ", at (" << current.getX() << ", " << current.getY();
    // std::cout << ") > " << val << " Area: " << ret.area << ", Perimeter: " << ret.perimeter << ", Corners: " << ret.corners << std::endl;

    return ret;
}

size_t
solve(const std::vector<std::string> &map, bool isPart2) {
    const size_t                  HEIGHT = map.size();
    const size_t                  WIDTH  = map[0].size();
    std::vector<std::vector<int>> visited(HEIGHT, std::vector<int>(WIDTH, 0));
    size_t                        ret = 0;

    for(size_t y = 0; y < HEIGHT; y++) {
        for(size_t x = 0; x < WIDTH; x++) {
            Region r = floodfill(map, visited, {(int)x, (int)y});
            if(!isPart2) {
                ret += r.area * r.perimeter;
            } else {
                ret += r.area * r.corners;
            }
        }
    }
    return ret;
}

size_t
part1(const std::vector<std::string> &lines) {
    return solve(lines, false);
}

size_t
part2(const std::vector<std::string> &lines) {
    return solve(lines, true);
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
    // 833808 too low
    return 0;
}