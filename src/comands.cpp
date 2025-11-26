#include "commands.h"
#include <iostream>
#include <string>
#include <unistd.h>
#include <limits.h>
#include <vector>

int Commands::echo(const std::vector<std::string>& args) {
    return 0;
}

int Commands::pwd(const std::vector<std::string>& args) {
    char cwd[PATH_MAX];
    std::cout << getcwd(cwd, sizeof(cwd)) << "\n";
    return 0;
}

int Commands::quit(const std::vector<std::string>& args) {
    std::cout << "Exiting MyShell...\n";
    std::exit(0); 
}
int Commands::clr(const std::vector<std::string>& args) {
    std::cout << "\033[H\033[J" << std::flush; //ANSI escape code which works on most terminals. It escapes with \033, then [H moves the cursor to the home position, and [J clears the screen from the cursor down.
    return 0;
}