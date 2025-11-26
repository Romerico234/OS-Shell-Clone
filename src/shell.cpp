#include <iostream>
#include <string>
#include <vector>
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "token.h"
#include "executor.h"
#include "commands.h"
#include <limits.h>
#include <unistd.h>

int main() {
    std::cout << "|  Welcome to our Custom Shell!\n";
    std::cout << "|  Type help for our list of commands!\n";

    while (true) {
        char cwd[PATH_MAX];
        getcwd(cwd, sizeof(cwd));
        std::cout << cwd << "/custom-shell> ";

        std::string input;
        std::getline(std::cin, input);

        if (input.empty()) {
            continue;
        }

        try {
            std::vector<Token> tokens = Lexer::tokenize(input);

            AST ast = Parser::parse(tokens);

            CommandResult result = Executor::executeCommand(ast);

            if (result.status == 0) {
                std::cout << result.output;
            }

            if (result.status == 1) {
                std::cerr << result.error;
            }

            std::cout << "\n";

        } catch (const std::exception& ex) {
            std::cerr << "Error: " << ex.what() << "\n";
        }
    }

    return 0;
}
