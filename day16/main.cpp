#include <util/StringUtil.hpp>
#include <util/FileUtil.hpp>
#include <parser/Tokenizer.hpp>
#include <math/Vector2.hpp>
#include <pathfinder/PathFinder.hpp>

#include <stdio.h>
#include <ctype.h>
#include <string>
#include <vector>
#include <set>
#include <iostream>

using namespace cam::util;
using namespace cam::parser;
using namespace cam::math;
using namespace cam::pathfinder;

class PathFinderTurns : public PathFinder {
protected:
    Node
    onStart(const Vector2 &start) const override {
        return {start, 0, 1, {start}};
    }

    double
    computeCost(const Node &current, const Vector2 &target) const override {
        double  ret   = PathFinder::computeCost(current, target);
        auto    dirs  = getValidDirections();
        Vector2 diff1 = target - current.pos;
        if(diff1 != dirs[current.dir]) {
            ret += 1000;
        }
        return ret;
    }
};

size_t
part1(const std::vector<std::string> &lines) {
    return 0;
}

size_t
part2(const std::vector<std::string> &lines) {
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

    std::vector<std::string> lines = StringUtil::split(content, "\n");
    PathFinderTurns          solver;
    solver.set(lines);
    auto path     = solver.solve();
    auto solution = solver.cost();
    // solver.dump();

    std::cout << "Part 1 total: " << solution << std::endl;

    std::vector<Vector2> uniques;
    auto                 list = solver.alternatives();
    for(const auto &l : list) {
        for(const auto &p : l) {
            if(std::find(uniques.begin(), uniques.end(), p) == uniques.end()) {
                uniques.push_back(p);
            }
        }
    }
    size_t p2 = uniques.size();
    std::cout << "Part 2 total: " << p2 << std::endl;

    return 0;
}