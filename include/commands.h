#pragma once
#include <vector>
#include <string>
#include <regex>

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
    static CommandResult pwdCommand(const std::vector<std::string>& args);
    static CommandResult clrCommand(const std::vector<std::string>& args);
    static CommandResult quitCommand(const std::vector<std::string>& args);
    static CommandResult environCommand(const std::vector<std::string>& args);
    static CommandResult catCommand(const std::vector<std::string>& args);
    static CommandResult wcCommand(const std::vector<std::string>& args);
    static CommandResult mkdirCommand(const std::vector<std::string>& args);
    static CommandResult rmCommand(const std::vector<std::string>& args);
    static CommandResult rmdirCommand(const std::vector<std::string>& args);
    static CommandResult touchCommand(const std::vector<std::string>& args);
    static CommandResult cpCommand(const std::vector<std::string>& args);
    static CommandResult chownCommand(const std::vector<std::string>& args);
    static CommandResult grepCommand(const std::vector<std::string>& args);
    static CommandResult mvCommand(const std::vector<std::string>& args);
    static CommandResult chmodCommand(const std::vector<std::string>& args);
    
private:
    static std::string formatLsLongListing(const std::string& name, const struct stat& info);
    static std::string formatRmdirErrorMsg(const std::string& path);
    static bool matchesPattern(const std::string& line, const std::regex& re, bool printOnlyMatch, std::string& outMatch);
    static std::string stripTrailingNewline(const std::string& s);
    static bool isFileEmpty(const std::string& filename);
}; 
