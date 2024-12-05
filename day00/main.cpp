#include <util/StringUtil.hpp>
#include <util/FileUtil.hpp>
#include <parser/Tokenizer.hpp>

#include <stdio.h>
#include <ctype.h>
#include <string>
#include <vector>

using namespace cam::util;
using namespace cam::parser;

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
        printf("[ERROR] Invalid input file: %s\n", asset.c_str());
        return -1;
    }

    std::vector<std::string> lines = StringUtil::split(content, "\n");

    size_t p1 = part1(lines);
    printf(" Part 1 total:  %lu\n", p1);

    size_t p2 = part2(lines);
    printf(" Part 2 total:  %lu\n", p2);

    return 0;
}