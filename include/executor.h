#pragma once
#include "ast.h"
#include "commands.h"

class Executor {
public:
    Executor() = delete;

    static CommandResult executeCommand(const AST& node);

private:
    static CommandResult execute(const AST& node);
    static CommandResult runCommand(const AST& node);

   /**
     * TODO:
     * The following operators are intentionally left unimplemented and are
     * provided as placeholders for future work.
     *
     * Supporting these features requires a broader redesign of the shell's
     * execution model so that all commands share consistent semantics for 
     * stdin/stdout, process creation, and control flow.
     */
    static CommandResult handlePipe(const AST& node);
    static CommandResult handleRedirectOut(const AST& node);
    static CommandResult handleRedirectIn(const AST& node);
    static CommandResult handleAppend(const AST& node);
    static CommandResult handleAnd(const AST& node);
    static CommandResult handleOr(const AST& node);
    static CommandResult handleSeq(const AST& node);
    static CommandResult handleBackground(const AST& node);
};
