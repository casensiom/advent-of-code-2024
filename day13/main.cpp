#include <util/StringUtil.hpp>
#include <util/FileUtil.hpp>
#include <parser/Tokenizer.hpp>

#include <stdio.h>
#include <ctype.h>
#include <string>
#include <vector>
#include <numeric>
#include <iostream>

using namespace cam::util;
using namespace cam::parser;


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
    std::vector<Game> ret;
    std::vector<std::string> groups = StringUtil::split(content, "\n\n");
    for(const std::string &g : groups) {
        Tokenizer tkn(g);
        Game game;
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

int64_t mod(int64_t a, int64_t m) {
    //int64_t ret = (a % m + m) % m;    // positive module
    int64_t ret = a % m;
    std::cout << a << " mod " << m << " == " << ret << std::endl;

    return ret; 
}

size_t
solve(const std::vector<Game> &games, bool isPart2) {
    size_t ret = 0;

    for(const Game &g : games) {

        // Button A: X+94, Y+34
        // Button B: X+22, Y+67
        // Prize: X=8400, Y=5400

        // X1 * a.x + X2 * b.x = prize.x
        // X1 * a.y + X2 * b.y = prize.y

        // X2 = (prize.x - X1 * a.x) / b.x
        // (X1 * a.y * b.x) - (X1 * a.x * b.y) = prize.y * b.x - (prize.x * b.y)
        // X1 * (a.y * b.x - a.x * b.y) = prize.y * b.x - (prize.x * b.y)
        // X1 = (prize.y * b.x - (prize.x * b.y)) / (a.y * b.x - a.x * b.y)
        
        std::cout << "---" << std::endl;
        int det = (g.a.y * g.b.x - g.a.x * g.b.y);
        if(det == 0) {
            continue;
        }

        v2 prize;
        prize.x = g.prize.x;
        prize.y = g.prize.y;
        if(isPart2) {
            prize.x = mod(prize.x + 10000000000000L, std::lcm(g.a.x, g.b.x));
            prize.y = mod(prize.y + 10000000000000L, std::lcm(g.a.y, g.b.y));
        }

        int X1 = ((prize.y * g.b.x) - (prize.x * g.b.y)) / det;
        int X2 = (prize.x - X1 * g.a.x) / g.b.x;

        std::cout << "X1 * " << g.a.x << " + X2 * " << g.b.x << " == " << prize.x << std::endl;
        std::cout << "X1 * " << g.a.y << " + X2 * " << g.b.y << " == " << prize.y << std::endl;
        std::cout << "  > " << X1 << " * " << g.a.x << " + " << X2 << " * " << g.b.x << " == " << prize.x << std::endl;
        std::cout << "  > " << X1 << " * " << g.a.y << " + " << X2 << " * " << g.b.y << " == " << prize.y << std::endl;

        if(X1 * g.a.x + X2 * g.b.x == prize.x && 
            X1 * g.a.y + X2 * g.b.y == prize.y 
            //&& X1 <= 100 && X2 <= 100
            ) {
            std::cout << X1 << " * " << g.a.x << " + " << X2 << " * " << g.b.x << " == " << prize.x << std::endl;
            std::cout << X1 << " * " << g.a.y << " + " << X2 << " * " << g.b.y << " == " << prize.y << std::endl;

            ret += X1 * 3 + X2;
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
    std::string asset = FileUtil::pathRemoveComponents(prg, 1) + "/assets/test01.txt";

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