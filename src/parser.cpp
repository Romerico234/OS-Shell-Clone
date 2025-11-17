#include "parser.h"
#include <stdexcept>

AST Parser::parse(const std::vector<Token>& tokens) {
    if (tokens.empty()) {
        throw std::runtime_error("Cannot parse empty token list");
    }

    int index = 0;
    AST tree = parse_command_line(index, tokens);
    return tree;
}

// <COMMAND_LINE> ::= <OP_EXPR>
AST Parser::parse_command_line(int& index, const std::vector<Token>& tokens) {
    AST lhs = parse_command_atomic(index, tokens);
    return parse_op_expression(std::move(lhs), 0, index, tokens);
}

// <COMMAND_ATOM> ::= <ARG_LIST>
AST Parser::parse_command_atomic(int& index, const std::vector<Token>& tokens) {
    int n = tokens.size();
    if (index >= n) {
        throw std::runtime_error("Unexpected end of input in command atom");
    }

    std::string cmd = tokens[index].lexeme;
    ++index;

    std::vector<std::string> args;

    while (index < n) {
        if (is_operator(tokens[index])) {
            break;
        }
        args.push_back(tokens[index].lexeme);
        ++index;
    }

    return AST::make_command_node(std::move(cmd), std::move(args));
}

// <OP_EXPR> ::= <lhs> OP <rhs> OP_TAIL (using precedence climbing)
AST Parser::parse_op_expression(AST lhs, int min_prec, int& index, const std::vector<Token>& tokens) {
    int n = tokens.size();

    while (index < n) {

        if (!is_operator(tokens[index])) {
            break;
        }

        const std::string& op = tokens[index].lexeme;
        int prec = precedence(op);

        if (prec < min_prec) { 
            break;
        }

        ++index;
        AST rhs = parse_command_atomic(index, tokens);

        while (index < n) {
            if (!is_operator(tokens[index])) {
                break;
            }

            std::string next = tokens[index].lexeme;
            int next_prec = precedence(next);

            if (next_prec > prec) {
                rhs = parse_op_expression(std::move(rhs), next_prec, index, tokens);
            } else {
                break;
            }
        }

        lhs = AST::make_binary_node(op, std::move(lhs), std::move(rhs));
    }

    return lhs;
}

bool Parser::is_operator(const Token& token) {
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
