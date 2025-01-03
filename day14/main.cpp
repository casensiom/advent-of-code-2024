#include <util/StringUtil.hpp>
#include <util/FileUtil.hpp>
#include <parser/Tokenizer.hpp>
#include <math/Vector2.hpp>

#include <stdio.h>
#include <ctype.h>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <limits>

using namespace cam::util;
using namespace cam::parser;
using namespace cam::math;

struct robot {
    Vector2i pos;
    Vector2i dir;
};

std::vector<robot>
parse(const std::string &content) {
    std::vector<robot> robots;
    Tokenizer          tkn(content);
    while(!tkn.eof()) {
        robot r;
        if(tkn.peek() != 'p') {
            std::cout << "[ERROR] Parsing data found not a 'p'." << std::endl;
            break;
        }
        tkn.consume(2);
        int pX = tkn.consumeInteger();
        tkn.consume(1);
        int pY = tkn.consumeInteger();
        tkn.consume(3);
        int dX = tkn.consumeInteger();
        tkn.consume(1);
        int dY = tkn.consumeInteger();
        tkn.consume(1);
        r.pos.set(pX, pY);
        r.dir.set(dX, dY);
        robots.push_back(r);
    }
    return robots;
}

int64_t
mod(int64_t a, int64_t m) {
    int64_t ret = (a % m + m) % m;    // positive module
    return ret;
}

size_t
solve(const std::vector<robot> &robots) {
    size_t  ret    = 0;
    int64_t width  = 11;
    int64_t height = 7;
    if(robots.size() > 12) {
        width  = 101;
        height = 103;
    }
    int64_t steps  = 100;
    int64_t quadr1 = 0;
    int64_t quadr2 = 0;
    int64_t quadr3 = 0;
    int64_t quadr4 = 0;

    for(const auto &r : robots) {
        int64_t x = mod((r.pos.getX() + r.dir.getX() * steps), width);
        int64_t y = mod((r.pos.getY() + r.dir.getY() * steps), height);
        if(x < width / 2 && y < height / 2) {
            quadr1++;
        } else if(x < width / 2 && y > height / 2) {
            quadr2++;
        } else if(x > width / 2 && y < height / 2) {
            quadr3++;
        } else if(x > width / 2 && y > height / 2) {
            quadr4++;
        }
    }
    std::cout << "q1: " << quadr1 << std::endl;
    std::cout << "q2: " << quadr2 << std::endl;
    std::cout << "q3: " << quadr3 << std::endl;
    std::cout << "q4: " << quadr4 << std::endl;

    return (quadr1 * quadr2 * quadr3 * quadr4);
}

void
dump(std::vector<std::vector<int>> map, int val) {
    size_t HEIGHT = map.size();
    size_t WIDTH  = map[0].size();

    std::cout << std::endl;
    for(size_t y = 0; y < HEIGHT; y++) {
        for(size_t x = 0; x < WIDTH; x++) {
            std::cout << ((map[y][x] == val) ? "#" : ".");
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

size_t
solve2(const std::vector<robot> &robots) {
    size_t  ret    = 0;
    int64_t width  = 11;
    int64_t height = 7;
    if(robots.size() > 12) {
        width  = 101;
        height = 103;
    }

    std::vector<robot> copy = robots;

    size_t best     = 0;
    int    minArea  = std::numeric_limits<int>::max();
    bool   simetric = false;
    size_t i        = 0;
    while(i < 10000) {
        ++i;

        std::vector<std::vector<int>> map(height, std::vector<int>(width, 0));
        for(auto &r : copy) {
            int pX = mod((r.pos.getX() + r.dir.getX()), width);
            int pY = mod((r.pos.getY() + r.dir.getY()), height);
            r.pos.set(pX, pY);

            map[r.pos.getY()][r.pos.getX()] = i;
        }

        for(size_t y = 0; y < height; y++) {
            int line = 0;
            for(size_t x = 0; x < width; x++) {
                if(map[y][x] == i) {
                    line++;
                    if(line >= 10) {
                        dump(map, i);
                        return i;
                    }
                } else {
                    line = 0;
                }
            }
        }
    }

    return 0;
}

size_t
part1(const std::vector<robot> &robots) {
    return solve(robots);
}

size_t
part2(const std::vector<robot> &robots) {
    return solve2(robots);
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

    std::vector<robot> robots = parse(content);

    size_t p1 = part1(robots);
    std::cout << "Part 1 total: " << p1 << std::endl;

    size_t p2 = part2(robots);
    std::cout << "Part 2 total: " << p2 << std::endl;

    return 0;
}