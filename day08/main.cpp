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
    int32_t x;
    int32_t y;
};
struct Antenna {
    v2            pos;
    unsigned char type;
};

struct Map {
    std::vector<Antenna> nodes;
    int32_t              width;
    int32_t              height;
    std::vector<Antenna> antinodes;

    bool
    inside(const v2 &pos) const {
        return (pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height);
    }

    bool
    find(const v2 &pos) const {
        bool found = false;
        for(const auto &item : antinodes) {
            if(item.pos.x == pos.x && item.pos.y == pos.y) {
                found = true;
                break;
            }
        }
        return found;
    }
};

Map
parse_input(const std::vector<std::string> &lines) {
    Map map;

    for(size_t y = 0; y < lines.size(); y++) {
        for(size_t x = 0; x < lines[y].size(); x++) {
            if(lines[y][x] == '.') {
                continue;
            } else {
                Antenna a;
                a.pos.x = x;
                a.pos.y = y;
                a.type  = lines[y][x];
                map.nodes.push_back(a);
            }
        }
    }

    map.width  = lines[0].size();
    map.height = lines.size();

    return map;
}

size_t
solve(Map map, bool isPart2) {
    for(size_t i = 0; i < map.nodes.size(); i++) {
        for(size_t j = 0; j < map.nodes.size(); j++) {
            if(i == j) {
                continue;
            }
            if(map.nodes[i].type == map.nodes[j].type) {
                int32_t dx = map.nodes[j].pos.x - map.nodes[i].pos.x;
                int32_t dy = map.nodes[j].pos.y - map.nodes[i].pos.y;

                int     step = (isPart2) ? 0 : 1;
                Antenna a;
                a.type  = map.nodes[i].type;
                a.pos.x = map.nodes[j].pos.x + dx * step;
                a.pos.y = map.nodes[j].pos.y + dy * step;
                do {
                    if(map.inside(a.pos) && !map.find(a.pos)) {
                        map.antinodes.push_back(a);
                    }
                    step++;
                    a.pos.x = map.nodes[j].pos.x + dx * step;
                    a.pos.y = map.nodes[j].pos.y + dy * step;
                } while(map.inside(a.pos) && isPart2);
            }
        }
    }
    return map.antinodes.size();
}

size_t
part1(Map map) {
    return solve(map, false);
}

size_t
part2(Map map) {
    return solve(map, true);
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
    Map                      map   = parse_input(lines);

    size_t p1 = part1(map);
    std::cout << "Part 1 total: " << p1 << std::endl;

    size_t p2 = part2(map);
    std::cout << "Part 2 total: " << p2 << std::endl;

    return 0;
}