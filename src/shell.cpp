#include <iostream>
#include <string>
#include <vector>
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "token.h"
#include "executor.h"

int main() {
    std::cout << "|  Welcome to our Custom Shell!\n";
    std::cout << "|  Type help for our list of commands!\n";

    while (true) {

        std::cout << "custom-shell> ";
        std::string input;
        std::getline(std::cin, input);

        if (input == "-1") { // Eventually changed to quit/exit command
            std::cout << "Exiting MyShell...\n";
            break;
        }

        if (input.empty()) {
            continue;
        }

        try {
            std::vector<Token> tokens = Lexer::tokenize(input);

            AST ast = Parser::parse(tokens);

            Executor::executeCommand(ast);

            std::cout << "\n";

        } catch (const std::exception& ex) {
            std::cerr << "Error: " << ex.what() << "\n\n";
        }
    }

    return 0;
}
