#pragma once
#include "ast.h"

class Executor {
public:
    Executor() = delete;

    static int executeCommand(const AST& node);

private:
    static int execute(const AST& node);
    static int runCommand(const AST& node);

    static int handlePipe(const AST& node);
    static int handleRedirectOut(const AST& node);
    static int handleRedirectIn(const AST& node);
    static int handleAppend(const AST& node);
    static int handleAnd(const AST& node);
    static int handleOr(const AST& node);
    static int handleSeq(const AST& node);
    static int handleBackground(const AST& node);
};
