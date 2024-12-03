#include <util/StringUtil.hpp>
#include <util/FileUtil.hpp>
#include <parser/Tokenizer.hpp>

#include <stdio.h>
#include <ctype.h>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace cam::util;
using namespace cam::parser;

bool
read_dont(Tokenizer &tokenizer, bool prevValue) {
    bool ret = true;
    if(!tokenizer.advanceIfEqual('d')) {
        return prevValue;
    }
    if(!tokenizer.advanceIfEqual('o')) {
        return prevValue;
    }

    if(tokenizer.peek() == 'n') {
        if(!tokenizer.advanceIfEqual('n')) {
            return prevValue;
        }
        if(!tokenizer.advanceIfEqual('\'')) {
            return prevValue;
        }
        if(!tokenizer.advanceIfEqual('t')) {
            return prevValue;
        }
        ret = false;
    }

    if(!tokenizer.advanceIfEqual('(')) {
        return prevValue;
    }
    if(!tokenizer.advanceIfEqual(')')) {
        return prevValue;
    }

    return ret;
}

size_t
read_mult(Tokenizer &tokenizer) {
    size_t ret = 0;

    // gets the instruction 'mul(xxx,yyy)'
    if(!tokenizer.advanceIfEqual('m')) {
        return 0;
    }
    if(!tokenizer.advanceIfEqual('u')) {
        return 0;
    }
    if(!tokenizer.advanceIfEqual('l')) {
        return 0;
    }
    if(!tokenizer.advanceIfEqual('(')) {
        return 0;
    }
    if(!tokenizer.isNumeric(tokenizer.peek())) {
        return 0;
    }
    tokenizer.consume();
    size_t num1 = tokenizer.consumeInteger();
    if(!tokenizer.advanceIfEqual(',')) {
        return 0;
    }
    if(!tokenizer.isNumeric(tokenizer.peek())) {
        return 0;
    }
    tokenizer.consume();
    size_t num2 = tokenizer.consumeInteger();
    if(!tokenizer.advanceIfEqual(')')) {
        return 0;
    }
    tokenizer.consume();

    ret += num1 * num2;
    return ret;
}

void
search_token(Tokenizer &tokenizer, bool isSecondPart) {
    if(isSecondPart) {
        while(!tokenizer.eof() && tokenizer.peek() != 'm' && tokenizer.peek() != 'd') {
            tokenizer.advance();
        }
    } else {
        while(!tokenizer.eof() && tokenizer.peek() != 'm') {
            tokenizer.advance();
        }
    }
    tokenizer.consume();
}

size_t
solve(Tokenizer tokenizer, bool isSecondPart) {
    size_t ret = 0;

    bool enableMul = true;
    while(!tokenizer.eof()) {
        // ignore previous data
        search_token(tokenizer, isSecondPart);

        if(isSecondPart && tokenizer.peek() == 'd') {
            enableMul = read_dont(tokenizer, enableMul);
        } else {
            size_t mulRes = read_mult(tokenizer);
            if(enableMul) {
                ret += mulRes;
            }
        }
    }

    return ret;
}

size_t
part1(const Tokenizer &tokenizer) {
    return solve(tokenizer, false);
}

size_t
part2(const Tokenizer &tokenizer) {
    return solve(tokenizer, true);
}

int
main(int argc, char **argv) {
    std::string prg   = argv[0];
    std::string asset = FileUtil::pathRemoveComponents(prg, -1) + "/assets/input01.txt";

    std::vector<uint8_t> fileContent = FileUtil::fileRead(asset);
    std::string          content(fileContent.begin(), fileContent.end());

    Tokenizer tokenizer(content);

    size_t p1 = part1(tokenizer);
    printf(" Part 1 total:  %lu\n", p1);

    size_t p2 = part2(tokenizer);
    printf(" Part 2 total:  %lu\n", p2);

    return 0;
}