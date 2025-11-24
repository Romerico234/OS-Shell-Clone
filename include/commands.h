#pragma once
#include <vector>
#include <string>

class Commands {
public:
    Commands() = delete;

    static int help(const std::vector<std::string>& args);
    static int echo(const std::vector<std::string>& args);
    static int pause(const std::vector<std::string>& args);
    static int ls(const std::vector<std::string>& args);
    static int dir(const std::vector<std::string>& args);
    static int cd(const std::vector<std::string>& args);
}; 
