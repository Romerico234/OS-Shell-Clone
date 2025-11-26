#pragma once
#include <string>
#include <vector>
#include <memory>
#include <ostream> 

class AST {
public:
    enum class NodeType {
        Command,
        Operator
    };

    NodeType node{};

    std::string command;
    std::vector<std::string> args;

    std::string op;
    std::unique_ptr<AST> left;
    std::unique_ptr<AST> right;

    static AST makeCommandNode(std::string cmd, std::vector<std::string> arguments);
    static AST makeOperatorNode(const std::string& op, AST lhs, AST rhs);
    void print(std::ostream& os, int indent = 0);

private:
    void indent(std::ostream& os, int n);
};
