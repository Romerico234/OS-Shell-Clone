#include "parser.h"
#include <stdexcept>

AST Parser::parse(const std::vector<Token>& tokens) {
    if (tokens.empty()) {
        throw std::runtime_error("Cannot parse empty token list");
    }

    int index = 0;
    AST tree = parseCmdLine(index, tokens);
    return tree;
}

// <COMMAND_LINE> ::= <OP_EXPR>
AST Parser::parseCmdLine(int& index, const std::vector<Token>& tokens) {
    AST lhs = parseCmdAtomic(index, tokens);
    return parseOpExpr(std::move(lhs), 0, index, tokens);
}

// <COMMAND_ATOM> ::= <ARG_LIST>
AST Parser::parseCmdAtomic(int& index, const std::vector<Token>& tokens) {
    int n = tokens.size();
    if (index >= n) {
        throw std::runtime_error("Unexpected end of input in command atom");
    }

    std::string cmd = tokens[index].lexeme;
    ++index;

    std::vector<std::string> args;

    while (index < n) {
        if (isOperator(tokens[index])) {
            break;
        }
        args.push_back(tokens[index].lexeme);
        ++index;
    }

    return AST::makeCommandNode(std::move(cmd), std::move(args));
}

// <OP_EXPR> ::= <lhs> OP <rhs> OP_TAIL (using precedence climbing)
AST Parser::parseOpExpr(AST lhs, int min_prec, int& index, const std::vector<Token>& tokens) {
    int n = tokens.size();

    while (index < n) {

        if (!isOperator(tokens[index])) {
            break;
        }

        const std::string& op = tokens[index].lexeme;
        int prec = precedence(op);

        if (prec < min_prec) { 
            break;
        }

        ++index;
        AST rhs = parseCmdAtomic(index, tokens);

        while (index < n) {
            if (!isOperator(tokens[index])) {
                break;
            }

            std::string next = tokens[index].lexeme;
            int next_prec = precedence(next);

            if (next_prec > prec) {
                rhs = parseOpExpr(std::move(rhs), next_prec, index, tokens);
            } else {
                break;
            }
        }

        lhs = AST::makeOperatorNode(op, std::move(lhs), std::move(rhs));
    }

    return lhs;
}

bool Parser::isOperator(const Token& token) {
    const std::string& op = token.lexeme;
    return (
        op == "||" || op == "&&" ||
        op == "|"  ||
        op == ">"  || op == ">>" || op == "<" ||
        op == ";" || op == "&"
    );
}

int Parser::precedence(const std::string& op) {
    if (op == "||") return 1;
    if (op == "&&") return 2;
    if (op == "|")  return 3;
    if (op == ">" || op == ">>" || op == "<") return 4;
    if (op == ";" || op == "&") return 0;
    return -1;
}
