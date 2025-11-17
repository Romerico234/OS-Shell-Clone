#include "ast.h"

AST AST::make_command_node(std::string cmd, std::vector<std::string> arguments) {
    AST node;
    node.node = NodeType::Command;
    node.command = std::move(cmd);
    node.args = std::move(arguments);
    return node;
}

AST AST::make_binary_node(const std::string& binOp, AST lhs, AST rhs) {
    AST node;
    node.node = NodeType::BinaryOp;
    node.binOp = binOp;
    node.left = std::make_unique<AST>(std::move(lhs));
    node.right = std::make_unique<AST>(std::move(rhs));
    return node;
}

void AST::indent(std::ostream& os, int n) {
    for (int i = 0; i < n; i++) {
        os << "  ";
    }
}

void AST::print(std::ostream& os, int indentLvl) {
    indent(os, indentLvl);

    if (node == NodeType::Command) {
        os << "Command: " << command;
        for (const std::string& a : args) {
            os << " [" << a << "]";
        }
        os << "\n";
    } else if (node == NodeType::BinaryOp) {
        os << "BinaryOp: '" << binOp << "'\n";
        if (left)  {
            left->print(os, indentLvl + 1);
        }
        if (right) {
            right->print(os, indentLvl + 1);
        }
    }
}
