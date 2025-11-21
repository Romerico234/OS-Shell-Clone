#pragma once
#include <string>
#include <vector>
#include <memory>
#include <ostream> 

class AST {
public:
    enum class NodeType {
        Command,
        BinaryOp
    };

    NodeType node{};

    std::string command;
    std::vector<std::string> args;

    std::string binOp;
    std::unique_ptr<AST> left;
    std::unique_ptr<AST> right;

    static AST makeCommandNode(std::string cmd, std::vector<std::string> arguments);
    static AST makeBinaryNode(const std::string& binOp, AST lhs, AST rhs);
    void print(std::ostream& os, int indent = 0);

private:
    void indent(std::ostream& os, int n);
};
