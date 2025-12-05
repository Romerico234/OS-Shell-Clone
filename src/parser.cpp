#include "parser.h"
#include <stdexcept>

AST Parser::parse(const std::vector<Token>& tokens) {
    if (tokens.empty()) {
        throw std::runtime_error("Cannot parse empty token list");
    }

    int index = 0;
    // <START> ::= <COMMAND_LINE> <END_OF_INPUT>
    // END_OF_INPUT is implicitly handled by reaching tokens.size()
    AST tree = parseCmdLine(index, tokens);
    return tree;
}

// <COMMAND_LINE> ::= <OP_EXPR>
AST Parser::parseCmdLine(int& index, const std::vector<Token>& tokens) {
    AST lhs = parseCmdAtomic(index, tokens);
    return parseOpExpr(std::move(lhs), 0, index, tokens);
}

// <COMMAND_ATOM> ::= <WORD_OR_QUOTED> <ARG_LIST>
// <ARG_LIST> implemented via a loop until an operator is seen
AST Parser::parseCmdAtomic(int& index, const std::vector<Token>& tokens) {
    int n = tokens.size();
    if (index >= n) {
        throw std::runtime_error("Unexpected end of input in command atom");
    }
    
    if (isOperator(tokens[index])) {
        throw std::runtime_error(
            "Expected command, found operator '" + tokens[index].lexeme + "'"
        );
    }

    // First element is the command name
    std::string cmd = tokens[index].lexeme;
    ++index;

    // Then collect arguments until we hit an operator
    std::vector<std::string> args;
    while (index < n && !isOperator(tokens[index])) {
        args.push_back(tokens[index].lexeme);
        ++index;
    }

    return AST::makeCommandNode(std::move(cmd), std::move(args));
}

/**
 * <OP_EXPR> ::= <COMMAND_ATOM> <OP_TAIL>
 *
 * <OP_TAIL> ::= <OPERATOR> <COMMAND_ATOM> <OP_TAIL> | ε
 *
 * Implemented via Precedence Climbing Method:
 *    - If next operator has higher precedence, recursively parse its RHS first
 *    - Otherwise return to the caller
 */
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

        // Handle higher-precedence operators on the RHS
        while (index < n && isOperator(tokens[index])) {

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
