#pragma once
#include <vector>
#include <string>
#include "ast.h"
#include "token.h"

/**
 * This project makes use of a Operator-Precednce Parser (uses precedence climbing which is specialized for infix expressions)
 * 
 * CFG:
 * <START> ::= <COMMAND_LINE> <END_OF_INPUT>
 * <COMMAND_LINE> ::= <OP_EXPR>
 * <OP_EXPR> ::= <COMMAND_ATOM> <OP_TAIL>
 * <OP_TAIL> ::= <OPERATOR> <COMMAND_ATOM> <OP_TAIL> | ε
 * <COMMAND_ATOM> ::= <WORD_OR_QUOTED> <ARG_LIST>
 * <ARG_LIST> ::= <WORD_OR_QUOTED> <ARG_LIST> | ε
 * <WORD_OR_QUOTED> ::= <WORD> | <QUOTED>
 * <OPERATOR> ::= '||' | '&&' | '|' | '>' | '>>' | '<' | ';' | '&'
 * <WORD> ::= any unquoted string of characters not matching <OPERATOR>
 * <QUOTED> ::= any string enclosed in single/double quotes
 * <END_OF_INPUT> ::= EOF token
 * 
 * Operator Precedence:
 * 4  ">", ">>", "<"
 * 3  "|"
 * 2  "&&"
 * 1  "||"
 * 0  ";", "&"
 * - All operators are left-associative e.g. a | b | c ::= (a | b) | c
*/

class Parser {
public:
    static AST parse(const std::vector<Token>& tokens);

private:
    static AST parseCmdLine(int& index, const std::vector<Token>& tokens);
    static AST parseOpExpr(AST lhs, int min_prec, int& index, const std::vector<Token>& tokens);
    static AST parseCmdAtomic(int& index, const std::vector<Token>& tokens);
    static bool isOperator(const Token& tok);
    static int precedence(const std::string& op);
};
