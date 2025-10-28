#include <iostream>     // to use std::cout and std::cin
#include <string>       // to handle text 
#include <vector>       // std::vector (stores command tokens)
#include <sstream>      // std::stringstream (splits user input into seperate tokens/words)
#include <filesystem>   // so we can use directories and files like in pwd and cd commands.

// Command Functions 
void pwd_command();                                     // runs the pwd command (Prints the current directory)                              
void cd_command(const std::vector<std::string>& args);  // runs the cd command to change directories.

// Handles first inpput of a user 
void handle_command(const std::vector<std::string>& tokens) {
    if (tokens.empty()) return;

    std::string cmd = tokens[0];                            // Loops if user typrs nothing and just hits enter. This prevents crashing or running a command when no input was entered.            

    if (cmd == "quit") {                                    // Quits program 
        std::cout << "Goodbye!\n";
        exit(0);
    } 
    else if (cmd == "pwd") {                                // Shows file path
        pwd_command();
    } 
    else if (cmd == "cd") {                                 // Changes directory
        cd_command(tokens);
    } 
    else {
        std::cout << "Command not found: " << cmd << "\n";  // Text if user types something that isnt a commdand.
    }
}

// Main shell loop
int main() {
    std::string input;

    std::cout << "You opened shell\nType 'quit' to exit.\n\n";  //

    while (true) {                                              // Keeps shell running until quit.
        std::cout << "Shell> ";
        std::getline(std::cin, input);

        if (std::cin.eof()) {
            std::cout << "\nExiting Shell...\n";                // End of file. Basically when the user tells the shell its done typing commands. (Usually Control+Z+Enter).
            break;
        }

        if (input.empty()) continue;

        // Split input into tokens
        std::stringstream ss(input);                    // Splits the iunput for exmaple: "cd Documents" becomes "["cd","Documents"]". Makes it easier to tell commands from arguments.
        std::string token;
        std::vector<std::string> tokens;
        while (ss >> token) tokens.push_back(token);    //

        handle_command(tokens);
    }

    return 0;
}

// Command Implementations 

// pwd_command()
void pwd_command() {
    try {
        std::filesystem::path cwd = std::filesystem::current_path();    // gets current directory path
        std::cout << cwd << "\n";                                       // prints to console
    } catch (std::filesystem::filesystem_error& e) {                    // Handles the errors 
        std::cout << "Error: " << e.what() << "\n";
    }
}

// cd_command()
void cd_command(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cout << std::filesystem::current_path() << "\n";   //
        return;
    }
    try {
        std::filesystem::current_path(args[1]);                 // Changes file path
    } catch (std::filesystem::filesystem_error& e) {            // Error if file path doesnt work
        std::cout << "cd: " << e.what() << "\n";
    }
}
