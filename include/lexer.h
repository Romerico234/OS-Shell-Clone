#pragma once
#include <string>
#include <vector>
#include "token.h"

class Lexer {
public:
    Lexer() = delete;

    static std::vector<Token> tokenize(const std::string& input);

private:
    static void flushCurrent(std::vector<Token>& tokens, std::string& current);
};
