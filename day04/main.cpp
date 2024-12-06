#include <util/StringUtil.hpp>
#include <util/FileUtil.hpp>

#include <stdio.h>
#include <ctype.h>
#include <string>
#include <array>
#include <vector>
#include <iostream>

using namespace cam::util;

size_t
count_all(const std::vector<std::string> &lines, int32_t x, int32_t y) {
    // XMAS
    // 1234

    // 4444444
    // 4333334
    // 4322234
    // 4321234
    // 4322234
    // 4333334
    // 4444444

    size_t h = lines.size();
    size_t w = lines[0].length();

    std::array<unsigned char, 4>    pattern = {'X', 'M', 'A', 'S'};
    std::array<bool, 9>             found   = {true, true, true, true, true, true, true, true, true};
    std::vector<std::string>        names   = {"WN", "N", "EN", "W", "-", "E", "WS", "S", "ES"};
    std::vector<std::array<int, 2>> dirs    = {
        {-1, -1},
        { 0, -1},
        { 1, -1},
        {-1,  0},
        { 0,  0},
        { 1,  0},
        {-1,  1},
        { 0,  1},
        { 1,  1}
    };

    for(int32_t d = 0; d < dirs.size(); d++) {
        for(int32_t pos = 0; pos < pattern.size(); pos++) {
            int32_t nx = x + dirs[d][0] * pos;
            int32_t ny = y + dirs[d][1] * pos;
            found[d] &= (nx >= 0 && nx < w && ny >= 0 && ny < h && lines[ny][nx] == pattern[pos]);
            if(!found[d]) {
                break;
            }
        }
    }

    size_t count = 0;
    for(int32_t pos = 0; pos < found.size(); pos++) {
        if(found[pos]) {
            count++;
        }
    }
    return count;
}

bool
count_cross(const std::vector<std::string> &lines, int32_t x, int32_t y) {
    size_t h = lines.size();
    size_t w = lines[0].length();
    if(x >= 1 && x < w - 1 && y >= 1 && y < h - 1) {
        unsigned char a = lines[y - 1][x - 1];
        unsigned char b = lines[y + 1][x + 1];

        unsigned char c = lines[y - 1][x + 1];
        unsigned char d = lines[y + 1][x - 1];

        return ((a == 'M' && b == 'S') || (a == 'S' && b == 'M')) && ((c == 'M' && d == 'S') || (c == 'S' && d == 'M'));
    }
    return false;
}

size_t
part1(const std::vector<std::string> &lines) {
    size_t ret = 0;

    size_t h = lines.size();
    size_t w = lines[0].length();

    for(size_t y = 0; y < h; y++) {
        for(size_t x = 0; x < w; x++) {
            if(lines[y][x] == 'X') {
                ret += count_all(lines, x, y);
            }
        }
    }

    return ret;
}

size_t
part2(const std::vector<std::string> &lines) {
    size_t ret = 0;

    size_t h = lines.size();
    size_t w = lines[0].length();

    for(size_t y = 0; y < h; y++) {
        for(size_t x = 0; x < w; x++) {
            if(lines[y][x] == 'A') {
                ret += count_cross(lines, x, y) ? 1 : 0;
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

    std::vector<std::string> lines = StringUtil::split(content, "\n");

    size_t p1 = part1(lines);
    std::cout << "Part 1 total: " << p1 << std::endl;

    size_t p2 = part2(lines);
    std::cout << "Part 2 total: " << p2 << std::endl;

    return 0;
}