#include <util/StringUtil.hpp>
#include <util/FileUtil.hpp>
#include <parser/Tokenizer.hpp>

#include <stdio.h>
#include <ctype.h>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>

using namespace cam::util;
using namespace cam::parser;

struct Program {
    int                  instructionPointer = 0;
    uint64_t             A                  = 0;
    uint64_t             B                  = 0;
    uint64_t             C                  = 0;
    std::vector<int64_t> data;
    std::vector<int>     out;

    int
    getCombo() {
        if(instructionPointer < 0 || instructionPointer >= data.size()) {
            return -1;
        }
        int val = data[instructionPointer];
        instructionPointer++;
        if(val <= 3) {
            return val;
        } else if(val == 4) {
            return A;
        } else if(val == 5) {
            return B;
        } else if(val == 6) {
            return C;
        }

        std::cout << "[ERROR] Value " << val << " found on a combo." << std::endl;
        exit(-1);
    }

    int
    getOpcode() {
        if(instructionPointer < 0 || instructionPointer >= data.size()) {
            return -1;
        }
        int ret = data[instructionPointer];
        instructionPointer++;
        return ret;
    }

    int
    getLiteral() {
        if(instructionPointer < 0 || instructionPointer >= data.size()) {
            return -1;
        }
        int ret = data[instructionPointer];
        instructionPointer++;
        return ret;
    }

    std::string
    getOutput() {
        std::string ret;
        for(auto v : out) {
            ret += std::to_string(v) + ",";
        }
        return ret;
    }

    std::string
    getInstructions() {
        std::string ret;
        for(auto v : data) {
            ret += std::to_string(v) + ",";
        }
        return ret;
    }

    int
    mod(int a, int m) {
        int ret = (a % m + m) % m;    // positive module
        return ret;
    }

    bool
    run() {
        std::map<std::tuple<int, int, int, int, int>, int> loopDetector;
        out.clear();
        instructionPointer = 0;
        while(instructionPointer >= 0 && instructionPointer < data.size()) {
            int opcode = getOpcode();

            switch(opcode) {
                case 0:    // adv
                    A = A / pow(2, getCombo());
                    break;
                case 1:    // bxl
                    B = B ^ getLiteral();
                    break;
                case 2:    // bst
                    B = mod(getCombo(), 8);
                    break;
                case 3:    // jnz
                    if(A != 0) {
                        int tmp = getLiteral();
                        // Check if the current status was already stored
                        std::tuple<int, int, int, int, int> state(A, B, C, instructionPointer, tmp);
                        if(loopDetector.find(state) == loopDetector.end()) {
                            loopDetector[state] = 1;
                        } else {
                            return true;
                        }

                        instructionPointer = tmp;
                    }
                    break;
                case 4:    // bxc
                    getLiteral();
                    B = C ^ B;
                    break;
                case 5:    // out
                    out.push_back(mod(getCombo(), 8));
                    break;
                case 6:    // bdv
                    B = A / pow(2, getCombo());
                    break;
                case 7:    // cdv
                    C = A / pow(2, getCombo());
                    break;
            }
        }
        return false;
    }
};

Program
parse(const std::string &content) {
    Program                  ret;
    std::vector<std::string> parts = StringUtil::split(content, "\n\n");

    Tokenizer tkn(parts[1]);
    tkn.consume(9);
    ret.data = tkn.consumeIntegerList(",");

    tkn.set(parts[0]);
    tkn.consume(12);
    ret.A = tkn.consumeInteger();
    tkn.consume(13);
    ret.B = tkn.consumeInteger();
    tkn.consume(13);
    ret.C = tkn.consumeInteger();

    return ret;
}

size_t
solve(Program &prg) {
    size_t ret = 0;
    return ret;
}

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

    Program program = parse(content);
    program.run();
    std::cout << "Part 1 total: " << program.getOutput() << std::endl;

    auto full = program.getInstructions();

    size_t   len   = 2;
    uint64_t val   = 0;
    int      loops = 0;
    bool     found = false;
    while(!found) {
        size_t      maxLen = std::min(full.size(), len);
        std::string target = StringUtil::substr(full, full.size() - maxLen, maxLen);
        // std::cout << "Partial: " << val << ", Len: " << maxLen << ", Target: " << target << " Full: " << full << " Len: " << full.size()
        // << std::endl;
        for(uint64_t i = 0; i < 8; i++) {
            program.A = val + i;
            program.B = 0;
            program.C = 0;
            program.run();
            std::string computed = program.getOutput();
            if(target == computed) {
                // std::cout << " > Found: " << i << ", Value" << (val | i) << ", found: " << computed << std::endl;
                val += i;
                if(len == full.size()) {
                    found = true;
                } else {
                    val <<= 3;
                    len += 2;
                }
                break;
            }
        }
    }

    std::cout << "Part 2 total: " << val << std::endl;

    return 0;
}