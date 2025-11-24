#include "commands.h"
#include <iostream>

int Commands::echo(const std::vector<std::string>& args) {
    return 0;
}

int Commands::pwd(const std::vector<std::string>& args) {
    return 0;
}

int Commands::quit(const std::vector<std::string>& args) {
    std::cout << "Exiting MyShell...\n";
    std::exit(0); 
}
