#pragma once
#include <vector>
#include <string>

class Commands {
public:
    Commands() = delete;

    static int echo(const std::vector<std::string>& args);
    static int pwd(const std::vector<std::string>& args);
};