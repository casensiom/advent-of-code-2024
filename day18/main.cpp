#include <util/StringUtil.hpp>
#include <util/FileUtil.hpp>
#include <parser/Tokenizer.hpp>
#include <pathfinder/PathFinder.hpp>
#include <math/Vector2.hpp>

#include <stdio.h>
#include <ctype.h>
#include <string>
#include <vector>
#include <iostream>
#include <chrono>

using namespace cam::util;
using namespace cam::parser;
using namespace cam::math;
using namespace cam::pathfinder;

std::vector<std::string>
parser(const std::string &content, int size, int fall) {
    std::vector<Vector2i> ret;
    Tokenizer             tkn(content);

    std::vector<std::string> map(size, std::string(size, '.'));

    while(!tkn.eof() && fall > 0) {
        int x = tkn.consumeInteger();
        tkn.consume(1);
        int y = tkn.consumeInteger();
        tkn.consume(1);
        map[y][x] = '#';
        fall--;
    }

    map[0][0]               = 'S';
    map[size - 1][size - 1] = 'E';

    return map;
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

    auto parts = StringUtil::split(content, "\n");

    int size  = 7;
    int fall  = 12;
    int lines = parts.size();
    if(lines > 25) {
        fall = 1024;
        size = 71;
    }

    PathFinder               pf;
    std::vector<std::string> map = parser(content, size, fall);
    pf.set(map);
    auto sol = pf.solve();
    std::cout << "Part 1 total: " << (sol.size() - 1) << std::endl;

    int lastValid = -1;
    int min       = 0;
    int max       = lines;
    while(min < max) {
        fall = (max + min) / 2;
        if(fall == min) {
            break;
        }
        map = parser(content, size, fall);
        pf.set(map);
        auto sol = pf.solve();
        if(sol.empty()) {
            max = fall;
        } else {
            // pf.dump();
            std::cout << "Iter: " << fall << ", Solution path: " << sol.size() << ", last piece: " << parts[fall] << ", limits: " << min
                      << ", " << max << std::endl;
            min       = fall;
            lastValid = fall;
        }
    }
    std::cout << "Part 2 total: " << parts[lastValid] << std::endl;

    return 0;
}