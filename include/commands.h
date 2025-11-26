#pragma once
#include <vector>
#include <string>

struct CommandResult {
    int status;
    std::string output;
    std::string error;
};

class Commands {
public:
    Commands() = delete;

    static CommandResult help(const std::vector<std::string>& args);
    static CommandResult echo(const std::vector<std::string>& args);
    static CommandResult pause(const std::vector<std::string>& args);
    static CommandResult ls(const std::vector<std::string>& args);
    static CommandResult dir(const std::vector<std::string>& args);
    static CommandResult cd(const std::vector<std::string>& args);

private:
    static int getCwd(char* buffer, size_t size);
}; 
