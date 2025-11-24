#include "executor.h"
#include "commands.h"
#include <iostream>

int Executor::executeCommand(const AST& node) {
    return execute(node);
}

int Executor::execute(const AST& node) {
    if (node.node == AST::NodeType::Command) {
        return runCommand(node);
    }

    const std::string& op = node.binOp;

    if (op == "|") {
        return handlePipe(node);
    } else if (op == ">") {
        return handleRedirectOut(node);
    } else if (op == "<") {
        return handleRedirectIn(node);
    } else if (op == ">>") {
        return handleAppend(node);
    } else if (op == "&&") {
        return handleAnd(node);
    } else if (op == "||") {
        return handleOr(node);
    } else if (op == ";") {
        return handleSeq(node);
    } else if (op == "&") {
        return handleBackground(node);
    } else {
        std::cerr << "Unknown bin operator: " << op << "\n";
        return -1;
    }
}

int Executor::runCommand(const AST& node) {
    if (node.command == "echo") {
        return Commands::echo(node.args);
    } else if (node.command == "pwd") {
        return Commands::pwd(node.args);
        } else if (node.command == "quit") {   
        return Commands::quit(node.args);
    } else {
        std::cerr << "Unknown command: " << node.command << "\n";
        return -1;
    }
}

int Executor::handlePipe(const AST& node) {
    return 0; 
}

int Executor::handleRedirectOut(const AST& node) {
    return 0;
}

int Executor::handleRedirectIn(const AST& node) {
    return 0;
}

int Executor::handleAppend(const AST& node) {
    return 0;
}

int Executor::handleAnd(const AST& node) {
    return 0;
}

int Executor::handleOr(const AST& node) {
    return 0;
}

int Executor::handleSeq(const AST& node) {
    return 0;
}

int Executor::handleBackground(const AST& node) {
    return 0;
}
