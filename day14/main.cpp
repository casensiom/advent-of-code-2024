#include <util/StringUtil.hpp>
#include <util/FileUtil.hpp>
#include <parser/Tokenizer.hpp>

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

struct v2 {
    int64_t x = 0;
    int64_t y = 0;

    v2() {
    }
    v2(int32_t x_, int32_t y_) : x(x_), y(y_) {
    }

    bool
    operator==(const v2 &other) const {
        return x == other.x && y == other.y;
    }
};

struct rect {
    v2 pos;
    v2 size;

    rect() {
    }
    rect(int64_t x, int64_t y, int64_t width, int64_t height) : pos(x, y), size(width, height) {
    }

};

struct robot {
    v2 pos;
    v2 dir;
};


std::vector<robot> 
parse(const std::string &content) {
    std::vector<robot> robots;
    Tokenizer tkn(content);
    while(!tkn.eof()) {
        robot r;
        if(tkn.peek() != 'p') {
            std::cout << "[ERROR] Parsing data found not a 'p'." << std::endl;
            break;
        }
        tkn.consume(2);
        r.pos.x = tkn.consumeInteger();
        tkn.consume(1);
        r.pos.y = tkn.consumeInteger();
        tkn.consume(3);
        r.dir.x = tkn.consumeInteger();
        tkn.consume(1);
        r.dir.y = tkn.consumeInteger();
        tkn.consume(1);
        robots.push_back(r);
    }
    return robots;
}

int64_t mod(int64_t a, int64_t m) {
    int64_t ret = (a % m + m) % m;    // positive module
    return ret; 
}

size_t
solve(const std::vector<robot> &robots) {
    size_t ret = 0;
    int64_t width = 11;
    int64_t height = 7;
    if(robots.size() > 12) {
        width = 101;
        height = 103;
    }
    int64_t steps = 100;
    int64_t quadr1 = 0;
    int64_t quadr2 = 0;
    int64_t quadr3 = 0;
    int64_t quadr4 = 0;

    
    for (const auto &r : robots) {
        int64_t x = mod((r.pos.x + r.dir.x * steps) , width);
        int64_t y = mod((r.pos.y + r.dir.y * steps) , height);
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
    
    return (quadr1*quadr2*quadr3*quadr4);
}


rect compute_area(const std::vector<v2>& elements) {
    int64_t minX = std::numeric_limits<int64_t>::max();
    int64_t maxX = std::numeric_limits<int64_t>::min();
    int64_t minY = std::numeric_limits<int64_t>::max();
    int64_t maxY = std::numeric_limits<int64_t>::min();
    
    for (const auto& e : elements) {
        minX = std::min(minX, e.x);
        maxX = std::max(maxX, e.x);
        minY = std::min(minY, e.y);
        maxY = std::max(maxY, e.y);
    }
    return rect(minX, minY, maxX - minX, maxY - minY);
}

// Calcula el área del rectángulo mínimo que contiene todos los puntos
int calculateArea(const std::vector<v2>& elements) {
    rect r = compute_area(elements);
    return (r.size.x + 1) * (r.size.y + 1);
}

void dump(const std::vector<v2>& elements) {
    rect r = compute_area(elements);
    std::vector<std::vector<char>> grid(r.size.y + 1, std::vector<char>(r.size.x + 1, '.'));
    for (const auto& e : elements) {
        grid[e.y - r.pos.y][e.x - r.pos.x] = '#';
    }
    
    for (const auto& row : grid) {
        for (char c : row) {
            std::cout << c;
        }
        std::cout << "\n";
    }
}

size_t
solve2(const std::vector<robot> &robots) {
    size_t ret = 0;
    int64_t width = 11;
    int64_t height = 7;
    if(robots.size() > 12) {
        width = 101;
        height = 103;
    }

    // int64_t steps = 1000;
    //for (size_t i = 0; i < steps; i++)

    size_t best = 0;
    int minArea = std::numeric_limits<int>::max();
    bool simetric = false;
    size_t i = 0;
    while(i < 100000)
    {
        ++i;

        std::vector<v2> pos;
        for (const auto &r : robots) {
            int64_t x = mod((r.pos.x + r.dir.x * i) , width);
            int64_t y = mod((r.pos.y + r.dir.y * i) , height);

            if(std::find(pos.begin(), pos.end(), v2(x, y)) != pos.end()) {
                continue;
            }
            pos.emplace_back(x, y);
        }

        int area = calculateArea(pos);
        if(area < minArea) {
            best = i;
            minArea = area;
            std::cout << "Time: " << i << ", Area: " << area << "\n";
            dump(pos);
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