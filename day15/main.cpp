#include <util/StringUtil.hpp>
#include <util/FileUtil.hpp>
#include <parser/Tokenizer.hpp>
#include <math/Vector2.hpp>

#include <stdio.h>
#include <ctype.h>
#include <string>
#include <vector>
#include <iostream>

using namespace cam::util;
using namespace cam::parser;
using namespace cam::math;

constexpr int EMPTY = 0x0;
constexpr int UP    = 0x1;
constexpr int RIGHT = 0x2;
constexpr int DOWN  = 0x4;
constexpr int LEFT  = 0x8;
constexpr int BLOCK = 0xF;

static std::vector<Vector2i> dirs = {
    { 0, -1},
    { 1,  0},
    { 0,  1},
    {-1,  0}
};

struct Puzzle {
    std::vector<std::string> data;
    std::vector<std::string> data2;
    std::string              instructions;
    Vector2i                 pos;
    size_t                   step = 0;
};

void
dump(const Puzzle &puzzle) {
    for(const auto &row : puzzle.data) {
        std::cout << row << std::endl;
    }
    std::cout << puzzle.instructions << std::endl;
    std::string spacer(puzzle.step, ' ');
    std::cout << spacer << "^" << std::endl;
}

Vector2i
find_start(const std::vector<std::string> &data) {
    size_t h = data.size();
    size_t w = data[0].size();

    for(size_t y = 0; y < h; y++) {
        for(size_t x = 0; x < w; x++) {
            if(data[y][x] == '@') {
                return {(int)x, (int)y};
            }
        }
    }

    return {0, 0};
}

std::vector<std::string>
extend(const std::vector<std::string> &data) {
    size_t                   HEIGHT = data.size();
    size_t                   WIDTH  = data[0].size();
    std::vector<std::string> ret;

    for(size_t y = 0; y < HEIGHT; y++) {
        std::string row;
        for(size_t x = 0; x < WIDTH; x++) {
            switch(data[y][x]) {
                case '#':
                    row += "##";
                    break;
                case 'O':
                    row += "[]";
                    break;
                case '.':
                    row += "..";
                    break;
                case '@':
                    row += "@.";
                    break;
            }
        }
        ret.push_back(row);
    }
    return ret;
}

Puzzle
parse(const std::string &content, bool isPart2) {
    Puzzle puzzle;
    auto   parts = StringUtil::split(content, "\n\n");
    if(parts.size() != 2) {
        std::cerr << "[ERROR] Error parsing input data." << std::endl;
    } else {
        puzzle.data = StringUtil::split(parts[0], "\n");
        if(isPart2) {
            puzzle.data = extend(puzzle.data);
        }
        puzzle.instructions = StringUtil::replace(parts[1], "\n", "");
        puzzle.step         = 0;
        puzzle.pos          = find_start(puzzle.data);
    }
    return puzzle;
}

unsigned char
get(const Puzzle &puzzle, const Vector2i &pos) {
    size_t h = puzzle.data.size();
    size_t w = puzzle.data[0].size();

    if(pos.getX() < 0 || pos.getX() >= w || pos.getY() < 0 || pos.getY() >= h) {
        return '#';
    }

    return puzzle.data[pos.getY()][pos.getX()];
}

// TODO: Convert to iterative
bool
check(const Puzzle &puzzle, Vector2i pos, size_t dir, int64_t level) {
    Vector2i      next = pos + dirs[dir] * level;
    unsigned char tile = get(puzzle, next);
    if(tile == '.') {
        return true;
    } else if(tile == '#') {
        return false;
    } else {
        bool ret = check(puzzle, pos, dir, level + 1);

        if(dir == 0 || dir == 2) {    // only vertical movements
            if(tile == '[') {
                ret &= check(puzzle, pos + Vector2i(-1, 0), dir, level + 1);
            } else if(tile == ']') {
                ret &= check(puzzle, pos + Vector2i(1, 0), dir, level + 1);
            }
        }
        return ret;
    }
}

void
add(std::vector<Vector2i> &vec, const Vector2i &pos) {
    if(std::find(vec.begin(), vec.end(), pos) == vec.end()) {
        vec.push_back(pos);
    }
}

bool
peek2(Puzzle &puzzle, size_t dir) {
    std::vector<Vector2i> queue;
    std::vector<Vector2i> positions;

    queue.push_back(puzzle.pos);
    positions.push_back(puzzle.pos);

    bool valid = true;
    while(!queue.empty()) {
        Vector2i current = queue.at(0);
        queue.erase(queue.begin());

        Vector2i next = current + dirs[dir];
        if(get(puzzle, next) == '.') {
            continue;
        }
        if(get(puzzle, next) == '#') {
            valid = false;
            break;
        }
        if(get(puzzle, next) == '[') {
            queue.push_back(next);
            queue.push_back(next + Vector2i(1, 0));
            add(positions, next);
            add(positions, next + Vector2i(1, 0));
        }
        if(get(puzzle, next) == ']') {
            queue.push_back(next);
            queue.push_back(next - Vector2i(1, 0));
            add(positions, next);
            add(positions, next - Vector2i(1, 0));
        }
    }

    if(valid) {
        std::sort(positions.begin(), positions.end(),
                  [dir](const Vector2i &a, const Vector2i &b) { return dir == 0 ? (a.getY() < b.getY()) : (a.getY() > b.getY()); });

        for(const auto &p : positions) {
            Vector2i      next                    = p + dirs[dir];
            unsigned char tmp                     = puzzle.data[p.getY()][p.getX()];
            puzzle.data[p.getY()][p.getX()]       = puzzle.data[next.getY()][next.getX()];
            puzzle.data[next.getY()][next.getX()] = tmp;
        }
        puzzle.pos = puzzle.pos + dirs[dir];
    }
    return valid;
}

bool
peek(const Puzzle &puzzle, size_t dir) {
    int64_t iter  = 0;
    bool    valid = false;
    while(!valid) {
        Vector2i      next = puzzle.pos + dirs[dir] * iter;
        unsigned char tile = get(puzzle, next);
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

void
move(Puzzle &puzzle, size_t dir) {
    int64_t iter = 0;
    while(get(puzzle, puzzle.pos + dirs[dir] * iter) != '.') {
        iter++;
    }
    while(iter > 0) {
        Vector2i      current                       = puzzle.pos + dirs[dir] * iter;
        Vector2i      prev                          = puzzle.pos + dirs[dir] * (iter - 1);
        unsigned char tmp                           = puzzle.data[prev.getY()][prev.getX()];
        puzzle.data[prev.getY()][prev.getX()]       = puzzle.data[current.getY()][current.getX()];
        puzzle.data[current.getY()][current.getX()] = tmp;
        iter--;
    }
    puzzle.pos = puzzle.pos + dirs[dir];
}

int
direction(Puzzle &puzzle) {
    unsigned char d   = puzzle.instructions[puzzle.step++];
    int           dir = 0;
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

void
advance(Puzzle &puzzle, bool isPart2) {
    int dir = direction(puzzle);
    if(isPart2 && (dir == 0 || dir == 2)) {
        peek2(puzzle, dir);
    } else {
        if(peek(puzzle, dir)) {
            move(puzzle, dir);
        }
    }
}

size_t
score(const Puzzle &puzzle) {
    size_t ret = 0;
    size_t h   = puzzle.data.size();
    size_t w   = puzzle.data[0].size();

    for(size_t y = 0; y < h; y++) {
        for(size_t x = 0; x < w; x++) {
            if(puzzle.data[y][x] == 'O' || puzzle.data[y][x] == '[') {
                ret += x + 100 * y;
            }
        }
    }
    return ret;
}

size_t
solve(Puzzle puzzle, bool isPart2) {
    while(puzzle.step < puzzle.instructions.size()) {
        advance(puzzle, isPart2);
        // dump(puzzle);
    }

    return score(puzzle);
}

size_t
part1(const Puzzle &puzzle) {
    return solve(puzzle, false);
}

size_t
part2(const Puzzle &puzzle) {
    return solve(puzzle, true);
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

    Puzzle puzzle;

    // puzzle = parse(content, false);
    // size_t p1 = part1(puzzle);
    // std::cout << "Part 1 total: " << p1 << std::endl;

    puzzle    = parse(content, true);
    size_t p2 = part2(puzzle);
    std::cout << "Part 2 total: " << p2 << std::endl;

    return 0;
}