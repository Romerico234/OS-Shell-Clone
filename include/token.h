#pragma once
#include <string>

enum class TokenType {
    WORD,           
    QUOTED,         
    AND_OP,         
    OR_OP,          
    APPEND_OP,      
    PIPE,           
    REDIR_OUT,      
    REDIR_IN,       
    SEMICOLON,      
    AMPERSAND,      
    END_OF_INPUT
};

struct Token {
    TokenType type;
    std::string lexeme;
};
