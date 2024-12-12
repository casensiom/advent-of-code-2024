#include <util/StringUtil.hpp>
#include <util/FileUtil.hpp>
#include <parser/Tokenizer.hpp>

#include <stdio.h>
#include <ctype.h>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace cam::util;
using namespace cam::parser;

struct v2 {
    int32_t x = 0;
    int32_t y = 0;

    v2() {
    }
    v2(int32_t x_, int32_t y_) : x(x_), y(y_) {
    }

    bool
    operator==(const v2 &other) const {
        return x == other.x && y == other.y;
    }
};

static std::vector<v2> dirs = {
    { 0, -1},
    { 1,  0},
    { 0,  1},
    {-1,  0}
};

std::vector<v2>
parse_input(const std::vector<std::string> &lines) {
    std::vector<v2> ret;
    for(size_t y = 0; y < lines.size(); y++) {
        for(size_t x = 0; x < lines[y].size(); x++) {
            if(lines[y][x] == '0') {
                ret.emplace_back(x, y);
            }
        }
    }
    return ret;
}

bool
can_move(const std::vector<std::string> &lines, const v2 &from, const v2 &to) {
    bool canMove = false;

    int32_t height = lines.size();
    int32_t width  = lines[0].size();

    if(to.x >= 0 && to.x < width && to.y >= 0 && to.y < height) {
        canMove = (lines[from.y][from.x] == (lines[to.y][to.x] - 1));
    }
    return canMove;
}

size_t
solve_single(std::vector<std::string> lines, const v2 &start, bool isPart2) {
    std::vector<v2> path;
    size_t          ret = 0;

    path.push_back(start);
    while(!path.empty()) {
        v2 current = path[0];
        path.erase(path.begin());
        if(lines[current.y][current.x] == '9') {
            if(!isPart2) {
                lines[current.y][current.x] = 'X';
            }
            ret++;
            continue;
        }
        for(const auto &d : dirs) {
            v2 next(current.x + d.x, current.y + d.y);
            if(can_move(lines, current, next)) {
                path.push_back(next);
            }
        }
    }

    return ret;
}

size_t
solve(std::vector<std::string> lines, const std::vector<v2> &starts, bool isPart2) {
    size_t ret = 0;
    for(const v2 &pos : starts) {
        ret += solve_single(lines, pos, isPart2);
    }
    return ret;
}

size_t
part1(const std::vector<std::string> &lines, const std::vector<v2> &starts) {
    return solve(lines, starts, false);
}

size_t
part2(const std::vector<std::string> &lines, const std::vector<v2> &starts) {
    return solve(lines, starts, true);
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

    std::vector<std::string> lines  = StringUtil::split(content, "\n");
    std::vector<v2>          starts = parse_input(lines);

    size_t p1 = part1(lines, starts);
    std::cout << "Part 1 total: " << p1 << std::endl;

    size_t p2 = part2(lines, starts);
    std::cout << "Part 2 total: " << p2 << std::endl;

    return 0;
}