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

struct v2 {
    int32_t x = 0;
    int32_t y = 0;
};

static std::vector<v2> dirs = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

struct Matrix {
    std::vector<std::vector<uint32_t>> data;

    size_t Width() const { return data[0].size(); }
    size_t Height() const { return data.size(); }

    void Set(size_t x, size_t y, uint32_t val) { data[y][x] |= val; }

    bool Check(size_t x, size_t y, uint32_t val) const {
        return data[y][x] == val;
    }

    bool CheckBit(size_t x, size_t y, uint32_t val) const {
        return (data[y][x] & val) != 0;
    }

    void Dump(size_t x_, size_t y_) const {
        static int step = 0;
        std::cout << std::endl;
        std::cout << "  [[ " << (++step) << " ]]" << std::endl;
        for (size_t y = 0; y < Height(); y++) {
            for (size_t x = 0; x < Width(); x++) {
                if (x == x_ && y == y_) {
                    std::cout << "O";
                } else if (data[y][x] == 0x90) {
                    std::cout << "#";
                } else if (data[y][x] == 0) {
                    std::cout << " ";
                } else if (data[y][x] == 1 || data[y][x] == 4) {
                    std::cout << "|";
                } else if (data[y][x] == 2 || data[y][x] == 8) {
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
    int32_t x = 0;
    int32_t y = 0;
    size_t dir = 0;

    Guard() = default;
    Guard(int32_t x, int32_t y, size_t dir) : x(x), y(y), dir(dir) {}

    void Move(int32_t steps) {
        auto p = dirs[dir];
        x += p.x * steps;
        y += p.y * steps;
    }
    void Forward() { Move(1); }
    void Backward() { Move(-1); }
    bool Inside(const Matrix &map) {
        return x >= 0 && x < map.Width() && y >= 0 && y < map.Height();
    }
    bool Blocked(const Matrix &map) {
        return Inside(map) && map.Check(x, y, 0x90);
    }
    void Turn() { dir = (dir + 1) % 4; }
};

struct State {
    Guard guard;
    Matrix map;

    bool Valid() { return guard.Inside(map); }

    void Step() {
        // map.data[guard.y][guard.x] |= (1 << guard.dir);    // mark
        map.Set(guard.x, guard.y, (1 << guard.dir));
        guard.Forward();
        if (guard.Blocked(map)) {
            guard.Backward();
            guard.Turn();
        }
    }
};

State get_position(std::vector<std::string> &lines) {
    State ret;

    for (size_t y = 0; y < lines.size(); y++) {
        std::vector<uint32_t> row;
        //(lines[y].size(), 0);
        for (size_t x = 0; x < lines[y].size(); x++) {
            row.push_back((lines[y][x] == '#') ? 0x90 : 0);
            if (lines[y][x] == '^') {
                ret.guard = Guard(x, y, 0);
            }
            if (lines[y][x] == '#') {
                lines[y][x] = 0x90;
            } else {
                lines[y][x] = 0;
            }
        }
        ret.map.data.push_back(row);
    }

    return ret;
}

bool check_stepped(const State &state) {
    return state.map.Check(state.guard.x, state.guard.y, 0);
}

bool check_simulation(const State &state) {
    Guard cp = state.guard;
    cp.Forward();
    if (!cp.Inside(state.map) || cp.Blocked(state.map)) {
        return false;
    }

    State copy = state;
    copy.guard.Turn();
    while (copy.Valid()) {
        copy.Step();
        if (copy.guard.Inside(copy.map) &&
            copy.map.CheckBit(copy.guard.x, copy.guard.y,
                              (1 << copy.guard.dir))) {
            // if (cp.x == 60) {

            //     std::cout << "Loop: " << cp.x << ", " << cp.y << std::endl;
            // }
            // copy.map.Dump(cp.x, cp.y);
            // exit(-1);
            return true;
        }
    }

    return false;
}

size_t solve(std::vector<std::string> lines, bool isPart2) {
    State state = get_position(lines);

    size_t ret = 0;
    while (state.Valid()) {
        if ((!isPart2 && check_stepped(state)) ||
            (isPart2 && check_simulation(state))) {
            ret++;
        }
        state.Step();
    }

    return ret;
}

size_t part1(std::vector<std::string> lines) { return solve(lines, false); }

size_t part2(std::vector<std::string> lines) { return solve(lines, true); }

int main(int argc, char **argv) {
    std::string prg = argv[0];
    std::string asset =
        FileUtil::pathRemoveComponents(prg, 1) + "/assets/input01.txt";

    std::vector<uint8_t> fileContent = FileUtil::fileRead(asset);
    std::string content(fileContent.begin(), fileContent.end());
    if (content.empty()) {
        std::cerr << "[ERROR] Invalid input file: " << asset << std::endl;
        return -1;
    }

    std::vector<std::string> map = StringUtil::split(content, "\n");

    size_t p1 = part1(map);
    std::cout << "Part 1 total: " << p1 << std::endl;

    size_t p2 = part2(map);
    std::cout << "Part 2 total: " << p2 << std::endl;
    // low 374
    // low 425
    // low 1000
    // invalid 1423

    return 0;
}