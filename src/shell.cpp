#include <iostream>
#include <string>
#include <vector>
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "token.h"

int main() {
    while (true) {
        std::cout << R"(
Testing Custom Shell. Enter any of the following commands or -1 to quit:
- cd
- rmdir
- touch
- cp
- chown
- ls / dir
- echo
- grep
- help
- pause
- pwd
- mkdir
- rm
- mv
- chmod
- cat
- wc
- environ
- clr
- quit
)" << std::endl;

        std::cout << "Enter command: ";

        std::string input;
        std::getline(std::cin, input);

        if (input == "-1") {
            std::cout << "Exiting shell..." << std::endl;
            break;
        }

        if (input.empty()) {
            continue;
        }

        try {
            std::vector<Token> tokens = Lexer::tokenize(input);

            std::cout << "\nTokens:\n";
            for (const Token& t : tokens) {
                std::cout << "  [" << t.lexeme << "]\n";
            }

            AST ast = Parser::parse(tokens);

            std::cout << "\nAST:\n";
            ast.print(std::cout); 

        } catch (const std::exception& ex) {
            std::cerr << "Parse error: " << ex.what() << "\n";
        }
    }

    return 0;
}
