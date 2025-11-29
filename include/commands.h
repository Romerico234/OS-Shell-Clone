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

static CommandResult helpCommand(const std::vector<std::string>& args);
static CommandResult echoCommand(const std::vector<std::string>& args);
static CommandResult pauseCommand(const std::vector<std::string>& args);
static CommandResult lsCommand(const std::vector<std::string>& args);
static CommandResult dirCommand(const std::vector<std::string>& args);
static CommandResult cdCommand(const std::vector<std::string>& args);
static CommandResult pwd(const std::vector<std::string>& args);
static CommandResult clr(const std::vector<std::string>& args);
static CommandResult quit(const std::vector<std::string>& args);
static CommandResult environ(const std::vector<std::string>& args);
static CommandResult cat(const std::vector<std::string>& args);
static CommandResult wc(const std::vector<std::string>& args);
static CommandResult mkdir(const std::vector<std::string>& args);
static CommandResult rm(const std::vector<std::string>& args);
static CommandResult rmdirCommand(const std::vector<std::string>& args);
static CommandResult touchCommand(const std::vector<std::string>& args);
static CommandResult cpCommand(const std::vector<std::string>& args);
static CommandResult chownCommand(const std::vector<std::string>& args);
static CommandResult grepCommand(const std::vector<std::string>& args);


private:
    static std::string formatRmdirErrorMsg(const std::string& path);
}; 
