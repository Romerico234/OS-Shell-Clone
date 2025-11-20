#pragma once
#include <vector>
#include <string>

class Commands {
public:
    Commands() = delete;

    static int cmd_echo(const std::vector<std::string>& args);
    static int cmd_pwd(const std::vector<std::string>& args);
};