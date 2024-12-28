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

constexpr int EMPTY = 0x0;
constexpr int UP    = 0x1;
constexpr int RIGHT = 0x2;
constexpr int DOWN  = 0x4;
constexpr int LEFT  = 0x8;
constexpr int BLOCK = 0xF;

struct v2 {
    int64_t x = 0;
    int64_t y = 0;

    v2() {
    }
    v2(int64_t x_, int64_t y_) : x(x_), y(y_) {
    }

    bool
    operator==(const v2 &other) const {
        return x == other.x && y == other.y;
    }

    v2
    operator*(int64_t scale) const {
        return {x * scale,  y * scale};
    }

    v2
    operator*(const v2 &other) const {
        return {x * other.x,  y * other.y};
    }

    v2
    operator+(const v2 &other) const {
        return {x + other.x,  y + other.y};
    }
};

static std::vector<v2> dirs = {
    { 0, -1},
    { 1,  0},
    { 0,  1},
    {-1,  0}
};

struct Puzzle {
    std::vector<std::string> data;
    std::string instructions;
    v2 pos;
    size_t step = 0;
};

v2
find_start(const std::vector<std::string> &data) {
    size_t h = data.size();
    size_t w = data[0].size();

    for (size_t y = 0; y < h; y++) {
        for (size_t x = 0; x < w; x++) {
            if(data[y][x] == '@') {
                return {(int64_t)x, (int64_t)y};
            }
        }
    }

    return {0, 0};
}

Puzzle
parse(const std::string &content) {
    Puzzle puzzle;
    auto parts = StringUtil::split(content, "\n\n");
    if(parts.size() != 2) {
        std::cerr << "[ERROR] Error parsing input data." << std::endl;
    } else {
        puzzle.data = StringUtil::split(parts[0], "\n");
        puzzle.instructions = StringUtil::replace(parts[1], "\n", "");
        puzzle.step = 0;
        puzzle.pos = find_start(puzzle.data);
    }
    return puzzle;
}

unsigned char
get(const Puzzle& puzzle, const v2 &pos) {
    size_t h = puzzle.data.size();
    size_t w = puzzle.data[0].size();

    if(pos.x <0 || pos.x >= w || pos.y <0 || pos.y >= h ) {
        return '#';
    }

    return puzzle.data[pos.y][pos.x];
}

//TODO: Convert to iterative
bool
check(const Puzzle &puzzle, v2 pos, size_t dir, int64_t level) {
    v2 next = pos + dirs[dir]*level;
    unsigned char tile = get(puzzle, next);
    if(tile == '.') {
        return true;
    } else if(tile == '#') {
        return false;
    } else {
        bool ret = check(puzzle, pos, dir, level+1);

        if(dir == 0 || dir == 2) { // only vertical movements
            if(tile == '[') {
                ret &= check(puzzle, pos + v2(-1, 0), dir, level+1);
            } else if(tile == ']') {
                ret &= check(puzzle, pos + v2(1, 0), dir, level+1);
            }
        }
        return ret;
    }
}


bool
peek(const Puzzle &puzzle, size_t dir) {
    int64_t iter = 0;
    bool valid = false;
    while(!valid) {
        v2 next = puzzle.pos + dirs[dir]*iter;
        unsigned char tile = get(puzzle,  next);
        if(tile == '.') {
            valid = true;
            break;
        } else if(tile == '#') {
            break;
        }
        iter++;
    }
    return valid;
}

void move(Puzzle& puzzle, size_t dir) {
    int64_t iter = 0;
    while(get(puzzle,  puzzle.pos + dirs[dir]*iter) != '.') {
        iter++;
    }
    while(iter > 0) {
        v2 current = puzzle.pos + dirs[dir]*iter;
        v2 prev = puzzle.pos + dirs[dir]*(iter - 1);
        unsigned char tmp = puzzle.data[prev.y][prev.x];
        puzzle.data[prev.y][prev.x] = puzzle.data[current.y][current.x];
        puzzle.data[current.y][current.x] = tmp;
        iter--;
    }
    puzzle.pos = puzzle.pos + dirs[dir];
}

int direction(Puzzle &puzzle) {
    unsigned char d = puzzle.instructions[puzzle.step++];
    int dir = 0;
    switch(d) {
        case '^':
        dir = 0;
        break;
        case '>':
        dir = 1;
        break;
        case 'v':
        dir = 2;
        break;
        case '<':
        dir = 3;
        break;
    }
    return dir;
}

void advance(Puzzle& puzzle) {
    int dir = direction(puzzle);
    if(peek(puzzle, dir)) {
        move(puzzle, dir);
    }
}

void dump(const Puzzle& puzzle) {
    for (const auto& row : puzzle.data) {
        std::cout << row << std::endl;
    }
    std::cout << puzzle.instructions << std::endl;
    std::string spacer(puzzle.step, ' ');
    std::cout << spacer << "^" << std::endl;
}


size_t
score(const Puzzle& puzzle) {
    size_t ret = 0;
    size_t h = puzzle.data.size();
    size_t w = puzzle.data[0].size();

    for (size_t y = 0; y < h; y++) {
        for (size_t x = 0; x < w; x++) {
            if(puzzle.data[y][x] == 'O') {    
                ret += x + 100 * y;
            }
        }
    }
    return ret;
}

size_t
solve(Puzzle puzzle) {
    while(puzzle.step < puzzle.instructions.size()) {
        advance(puzzle);
        // dump(puzzle);
    }

    return score(puzzle);
}

size_t
part1(const Puzzle &puzzle) {
    return solve(puzzle);
}

size_t
part2(const Puzzle &puzzle) {
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

    Puzzle puzzle = parse(content);


    size_t p1 = part1(puzzle);
    std::cout << "Part 1 total: " << p1 << std::endl;

    size_t p2 = part2(puzzle);
    std::cout << "Part 2 total: " << p2 << std::endl;

    return 0;
}