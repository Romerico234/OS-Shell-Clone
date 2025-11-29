#include "executor.h"
#include "commands.h"
#include <iostream>

CommandResult Executor::executeCommand(const AST& node) {
    return execute(node);
}

CommandResult Executor::execute(const AST& node) {
    if (node.node == AST::NodeType::Command)
        return runCommand(node);

    const std::string& op = node.op;

    if (op == "|")  return handlePipe(node);
    if (op == ">")  return handleRedirectOut(node);
    if (op == "<")  return handleRedirectIn(node);
    if (op == ">>") return handleAppend(node);
    if (op == "&&") return handleAnd(node);
    if (op == "||") return handleOr(node);
    if (op == ";")  return handleSeq(node);
    if (op == "&")  return handleBackground(node);

    return {1, "", "Unknown operator: " + op};
}

CommandResult Executor::runCommand(const AST& node) {
    if (node.command == "help") return Commands::help(node.args);
    if (node.command == "echo") return Commands::echo(node.args);
    if (node.command == "pause") return Commands::pause(node.args);
    if (node.command == "ls")   return Commands::ls(node.args);
    if (node.command == "dir")  return Commands::dir(node.args);
    if (node.command == "cd")   return Commands::cd(node.args);
    if (node.command == "pwd")  return Commands::pwd(node.args);
    if (node.command == "clr")  return Commands::clr(node.args);
    if (node.command == "quit") return Commands::quit(node.args);
    if (node.command == "environ") return Commands::environ(node.args);
    if (node.command == "cat") return Commands::cat(node.args);
    if (node.command == "wc") return Commands::wc(node.args);
    if (node.command == "mkdir") return Commands::mkdir(node.args);
    if (node.command == "rm") return Commands::rm(node.args);
    return {1, "", "Unknown command: " + node.command};
}

CommandResult Executor::handlePipe(const AST& node) {
    return {1, "", ""}; 
}

CommandResult Executor::handleRedirectOut(const AST& node) {
    return {1, "", ""};
}

CommandResult Executor::handleRedirectIn(const AST& node) {
    return {1, "", ""};
}

CommandResult Executor::handleAppend(const AST& node) {
    return {1, "", ""};
}

CommandResult Executor::handleAnd(const AST& node) {
    return {1, "", ""};
}

CommandResult Executor::handleOr(const AST& node) {
    return {1, "", ""};
}

CommandResult Executor::handleSeq(const AST& node) {
    return {1, "", ""};
}

CommandResult Executor::handleBackground(const AST& node) {
    return {1, "", ""};
}
