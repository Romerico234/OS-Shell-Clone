#include "lexer.h"
#include <unordered_map>
#include <cctype>

using OpMap = std::unordered_map<std::string, TokenType>;
using OpIter = OpMap::const_iterator;

static const OpMap OP_TABLE = {
    {"&&", TokenType::AND_OP},
    {"||", TokenType::OR_OP},
    {">>", TokenType::APPEND_OP},
    {"|",  TokenType::PIPE},
    {">",  TokenType::REDIR_OUT},
    {"<",  TokenType::REDIR_IN},
    {";",  TokenType::SEMICOLON},
    {"&",  TokenType::AMPERSAND}
};

void Lexer::flushCurrent(std::vector<Token>& tokens, std::string& current) {
    if (!current.empty()) {
        tokens.push_back({TokenType::WORD, current});
        current.clear();
    }
}

std::vector<Token> Lexer::tokenize(const std::string& input) {
    std::vector<Token> tokens;
    std::string current;

    int i = 0;
    const int n = static_cast<int>(input.size());

    while (i < n) {
        char c = input[i];

        if (std::isspace(static_cast<unsigned char>(c))) { // whitespace
            flushCurrent(tokens, current);
            ++i;
        } else if (c == '"' || c == '\'') { // quotes
            char quote = c;
            ++i; // skip opening quote

            std::string quoted;
            while (i < n && input[i] != quote) {
                quoted.push_back(input[i]);
                ++i;
            }

            if (i < n) {
                ++i; // skip closing quote
            }

            flushCurrent(tokens, current);
            tokens.push_back({TokenType::QUOTED, quoted});
        } else if (i + 1 < n) { // possible multi-char op
            std::string two = input.substr(i, 2);

            OpIter multiOpIter = OP_TABLE.find(two);

            if (multiOpIter != OP_TABLE.end()) { // matched multi-char op
                flushCurrent(tokens, current);
                tokens.push_back({multiOpIter->second, two});
                i += 2;
            } else {
                std::string one = input.substr(i, 1);

                OpIter iter = OP_TABLE.find(one);

                if (iter != OP_TABLE.end()) { // check single-char op 
                    flushCurrent(tokens, current);
                    tokens.push_back({iter->second, one});
                    ++i;
                } else { // normal word token
                    current.push_back(c);
                    ++i;
                }
            }
        } else { // normal word char
            current.push_back(c);
            ++i;
        }
    }

    flushCurrent(tokens, current);
    return tokens;
}
