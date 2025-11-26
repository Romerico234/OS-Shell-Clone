#include <iostream>
#include <string>
#include <vector>
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "token.h"
#include "executor.h"
#include "commands.h"

int main() {
    std::cout << "|  Welcome to our Custom Shell!\n";
    std::cout << "|  Type help for our list of commands!\n";

    while (true) {

        std::cout << "custom-shell> ";
        std::string input;
        std::getline(std::cin, input);

        if (input.empty()) {
            continue;
        }

        try {
            std::vector<Token> tokens = Lexer::tokenize(input);

            AST ast = Parser::parse(tokens);

            CommandResult result = Executor::executeCommand(ast);

            if (!result.output.empty()) {
                std::cout << result.output;
            }

            if (!result.error.empty()) {
                std::cerr << result.error;
            }

            std::cout << "\n";

        } catch (const std::exception& ex) {
            std::cerr << "Error: " << ex.what() << "\n";
        }
    }

    return 0;
}
