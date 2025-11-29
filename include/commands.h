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
    static CommandResult pwd(const std::vector<std::string>& args);
    static CommandResult clr(const std::vector<std::string>& args);
    static CommandResult quit(const std::vector<std::string>& args);    
    static CommandResult environ(const std::vector<std::string>& args);
    static CommandResult cat(const std::vector<std::string>& args);
    static CommandResult wc(const std::vector<std::string>& args);
    static CommandResult mkdir(const std::vector<std::string>& args);
    static CommandResult rm(const std::vector<std::string>& args);
private:
    static int getCwd(char* buffer, size_t size);
}; 
