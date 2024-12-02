#include <util/StringUtil.hpp>
#include <util/FileUtil.hpp>

#include <stdio.h>
#include <ctype.h>
#include <string>
#include <vector>

using namespace cam::util;

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
    std::vector<uint8_t>     fileContent = FileUtil::fileRead("assets/test01.txt");
    std::string              content(fileContent.begin(), fileContent.end());
    std::vector<std::string> lines = StringUtil::split(content, "\n");

    size_t p1 = part1(lines);
    printf(" Part 1 total:  %lu\n", p1);

    size_t p2 = part2(lines);
    printf(" Part 2 total:  %lu\n", p2);

    return 0;
}