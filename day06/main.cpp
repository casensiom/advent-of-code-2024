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

struct v2 {
    int32_t x = 0;
    int32_t y = 0;
};

struct Guard {
    Guard() = default;
    Guard(int32_t x, int32_t y, size_t dir) : x(x), y(y), dir(dir) {
    }
    int32_t x   = 0;
    int32_t y   = 0;
    size_t  dir = 0;
};

static std::vector<v2> dirs = {
    { 0, -1},
    { 1,  0},
    { 0,  1},
    {-1,  0}
};

static std::vector<unsigned char> chrs = {'^', '>', 'v', '<'};

Guard
get_position(const std::vector<std::string> &lines) {
    for(size_t y = 0; y < lines.size(); y++) {
        for(size_t x = 0; x < lines[y].size(); x++) {
            if(lines[y][x] == '^') {
                return Guard(x, y, 0);
            }
        }
    }

    return {};
}

bool
inside(const std::vector<std::string> &lines, const Guard &guard) {
    return guard.y >= 0 && guard.y < lines.size() && guard.x >= 0 && guard.x < lines[0].length();
}

void
advance(std::vector<std::string> &lines, Guard &guard) {
    auto p = dirs[guard.dir];
    guard.x += p.x;
    guard.y += p.y;

    if(lines[guard.y][guard.x] == '#') {
        guard.x -= p.x;
        guard.y -= p.y;
        guard.dir = (guard.dir + 1) % 4;
        p         = dirs[guard.dir];
        guard.x += p.x;
        guard.y += p.y;
    }
}

bool
check_projection(const std::vector<std::string> &lines, Guard copy) {
    Guard cp = copy;
    auto  p  = dirs[copy.dir];
    cp.x += p.x;
    cp.y += p.y;
    if(!inside(lines, cp)) {
        return false;
    }

    copy.dir = (copy.dir + 1) % 4;
    while(inside(lines, copy) && lines[copy.y][copy.x] != '#') {
        copy.x += p.x;
        copy.y += p.y;
        if(lines[copy.y][copy.x] == chrs[copy.dir]) {
            return true;
        }
    }

    return false;
}

size_t
part1(std::vector<std::string> lines) {
    Guard guard = get_position(lines);

    size_t ret = 0;
    while(inside(lines, guard)) {
        if(lines[guard.y][guard.x] == '.') {
            ret++;
        }
        lines[guard.y][guard.x] = 'X';    // mark
        advance(lines, guard);
    }

    return ret + 1;
}

size_t
part2(std::vector<std::string> lines) {
    Guard guard = get_position(lines);

    size_t ret = 0;
    while(inside(lines, guard)) {
        if(lines[guard.y][guard.x] == '.' && check_projection(lines, guard)) {
            ret++;
        }
        lines[guard.y][guard.x] = chrs[guard.dir];    // mark
        advance(lines, guard);
    }

    return ret;
}

int
main(int argc, char **argv) {
    std::string prg   = argv[0];
    std::string asset = FileUtil::pathRemoveComponents(prg, 1) + "/assets/test01.txt";

    std::vector<uint8_t> fileContent = FileUtil::fileRead(asset);
    std::string          content(fileContent.begin(), fileContent.end());
    if(content.empty()) {
        std::cerr << "[ERROR] Invalid input file: " << asset << std::endl;
        return -1;
    }

    std::vector<std::string> map = StringUtil::split(content, "\n");

    size_t p1 = part1(map);
    std::cout << "Part 1 total: " << p1 << std::endl;

    size_t p2 = part2(map);
    std::cout << "Part 2 total: " << p2 << std::endl;

    return 0;
}