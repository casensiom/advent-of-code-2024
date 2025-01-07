#include <util/StringUtil.hpp>
#include <util/FileUtil.hpp>
#include <parser/Tokenizer.hpp>
#include <math/MathUtil.hpp>

#include <stdio.h>
#include <ctype.h>
#include <string>
#include <vector>
#include <numeric>
#include <iostream>

using namespace cam::util;
using namespace cam::parser;
using namespace cam::math;

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

struct Game {
    v2 a;
    v2 b;
    v2 prize;
};

std::vector<Game>
parse(const std::string &content) {
    std::vector<Game>        ret;
    std::vector<std::string> groups = StringUtil::split(content, "\n\n");
    for(const std::string &g : groups) {
        Tokenizer tkn(g);
        Game      game;
        tkn.consume(12);
        game.a.x = tkn.consumeInteger();
        tkn.consume(4);
        game.a.y = tkn.consumeInteger();
        tkn.consume(13);
        game.b.x = tkn.consumeInteger();
        tkn.consume(4);
        game.b.y = tkn.consumeInteger();
        tkn.consume(10);
        game.prize.x = tkn.consumeInteger();
        tkn.consume(4);
        game.prize.y = tkn.consumeInteger();

        ret.push_back(game);
    }
    return ret;
}

size_t
solve(const std::vector<Game> &games, bool isPart2) {
    size_t ret = 0;

    // Represent the system as matrix
    // |x1 y1|  |A| _ |d1|
    // |x2 y2|  |B| - |d2|

    // X * M = D   -> X = M^-1 * D

    // M^-1 =  1/det(M) * | y2 -y1|
    //                    |-x2  y2|

    // A = (y2*d1 - y1*d2) / det(M)
    // A = (-x2*d1 + x1*d2) / det(M)

    for(const Game &g : games) {
        int64_t det = (g.a.x * g.b.y - g.b.x * g.a.y);

        v2 prize;
        prize.x = g.prize.x;
        prize.y = g.prize.y;
        if(isPart2) {
            prize.x = prize.x + 10000000000000L;
            prize.y = prize.y + 10000000000000L;
        }

        if(isNotZero(det)) {
            int64_t A = (prize.x * g.b.y - g.b.x * prize.y) / det;
            int64_t B = (g.a.x * prize.y - prize.x * g.a.y) / det;

            if(A * g.a.x + B * g.b.x != prize.x || A * g.a.y + B * g.b.y != prize.y) {
                continue;
            }
            if(!isPart2 && (A > 100 || B > 100)) {
                continue;
            }

            ret += A * 3 + B;
        } else {
            std::cout << " Sin solucion." << std::endl;
        }
    }

    return ret;
}

size_t
part1(const std::vector<Game> &games) {
    return solve(games, false);
}

size_t
part2(const std::vector<Game> &games) {
    return solve(games, true);
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

    std::vector<Game> games = parse(content);

    size_t p1 = part1(games);
    std::cout << "Part 1 total: " << p1 << std::endl;

    size_t p2 = part2(games);
    std::cout << "Part 2 total: " << p2 << std::endl;

    return 0;
}