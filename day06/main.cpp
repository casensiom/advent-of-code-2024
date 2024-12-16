#include <ctype.h>
#include <stdio.h>

#include <iostream>
#include <parser/Tokenizer.hpp>
#include <string>
#include <util/FileUtil.hpp>
#include <util/StringUtil.hpp>
#include <vector>

using namespace cam::util;
using namespace cam::parser;

constexpr int EMPTY = 0x0;
constexpr int UP    = 0x1;
constexpr int RIGHT = 0x2;
constexpr int DOWN  = 0x4;
constexpr int LEFT  = 0x8;
constexpr int BLOCK = 0xF;

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

struct Matrix {
    std::vector<std::vector<uint32_t>> data;

    size_t
    Width() const {
        return data[0].size();
    }
    size_t
    Height() const {
        return data.size();
    }

    void
    Set(size_t x, size_t y, uint32_t val) {
        data[y][x] |= val;
    }

    bool
    Check(size_t x, size_t y, uint32_t val) const {
        return data[y][x] == val;
    }

    bool
    CheckBit(size_t x, size_t y, uint32_t val) const {
        return (data[y][x] & val) != 0;
    }

    void
    Dump(size_t x_, size_t y_) const {
        static int step = 0;
        std::cout << std::endl;
        std::cout << "  [[ " << (++step) << " ]]" << std::endl;
        for(size_t y = 0; y < Height(); y++) {
            for(size_t x = 0; x < Width(); x++) {
                if(x == x_ && y == y_) {
                    std::cout << "O";
                } else if(data[y][x] == BLOCK) {
                    std::cout << "#";
                } else if(data[y][x] == EMPTY) {
                    std::cout << " ";
                } else if(data[y][x] == UP || data[y][x] == DOWN) {
                    std::cout << "|";
                } else if(data[y][x] == RIGHT || data[y][x] == LEFT) {
                    std::cout << "-";
                } else {
                    std::cout << "+";
                }
            }
            std::cout << std::endl;
        }
    }
};

struct Guard {
    int32_t x   = 0;
    int32_t y   = 0;
    size_t  dir = 0;

    Guard() = default;
    Guard(int32_t x, int32_t y, size_t dir) : x(x), y(y), dir(dir) {
    }

    v2
    Position() const {
        v2 pos;
        pos.x = x;
        pos.y = y;
        return pos;
    }

    void
    Move(int32_t steps) {
        auto p = dirs[dir];
        x += p.x * steps;
        y += p.y * steps;
    }
    void
    Forward() {
        Move(1);
    }
    void
    Backward() {
        Move(-1);
    }
    bool
    Inside(const Matrix &map) {
        return x >= 0 && x < map.Width() && y >= 0 && y < map.Height();
    }
    bool
    Blocked(const Matrix &map) {
        return Inside(map) && map.Check(x, y, BLOCK);
    }
    void
    Turn() {
        dir = (dir + 1) % 4;
    }

    bool
    Peek(const Matrix &map) {
        Forward();
        bool ret = (Inside(map) && !Blocked(map));
        Backward();
        return ret;
    }
};

struct State {
    Guard  guard;
    Matrix map;

    bool
    Valid() {
        return guard.Inside(map);
    }

    bool
    Visited() {
        return map.CheckBit(guard.x, guard.y, (1 << guard.dir));
    }

    void
    Step() {
        map.Set(guard.x, guard.y, (1 << guard.dir));
        guard.Forward();
        if(guard.Blocked(map)) {
            guard.Backward();
            guard.Turn();
        }
    }
};

State
get_position(std::vector<std::string> &lines) {
    State ret;

    for(size_t y = 0; y < lines.size(); y++) {
        std::vector<uint32_t> row;
        for(size_t x = 0; x < lines[y].size(); x++) {
            row.push_back((lines[y][x] == '#') ? BLOCK : EMPTY);
            if(lines[y][x] == '^') {
                ret.guard = Guard(x, y, 0);
            }
            if(lines[y][x] == '#') {
                lines[y][x] = BLOCK;
            } else {
                lines[y][x] = EMPTY;
            }
        }
        ret.map.data.push_back(row);
    }

    return ret;
}

size_t
solve(std::vector<std::string> lines) {
    State state = get_position(lines);

    size_t ret = 0;
    while(state.Valid()) {
        if(state.map.Check(state.guard.x, state.guard.y, EMPTY)) {
            ret++;
        }
        state.Step();
    }

    return ret;
}

size_t
solve2(std::vector<std::string> lines) {
    size_t ret    = 0;
    size_t height = lines.size();
    size_t width  = lines[0].size();
    State  orig   = get_position(lines);

    for(size_t y = 0; y < height; y++) {
        for(size_t x = 0; x < width; x++) {
            State state = orig;
            if(state.map.Check(x, y, BLOCK) || v2(x, y) == state.guard.Position()) {
                continue;
            }
            state.map.data[y][x] = BLOCK;

            while(state.Valid()) {
                if(state.Visited()) {
                    ret++;
                    break;
                }
                state.Step();
            }
        }
    }

    return ret;
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

    std::vector<std::string> map = StringUtil::split(content, "\n");

    size_t p1 = solve(map);
    std::cout << "Part 1 total: " << p1 << std::endl;

    size_t p2 = solve2(map);
    std::cout << "Part 2 total: " << p2 << std::endl;

    return 0;
}