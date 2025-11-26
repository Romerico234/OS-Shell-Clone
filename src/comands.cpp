#include "commands.h"
#include <limits>
#include <string>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <iostream>

/**
 * @brief Display a list of all supported shell commands
 * @param args Must be empty
 * @return Status code and a help message on success
 */
CommandResult Commands::help(const std::vector<std::string>& args) {
    if (!args.empty()) {
        return {1, "", "help: this command takes no arguments"};
    }

    std::string out =
        "Available Commands:\n"
        "  cd [dir]              Change directory.\n"
        "  clr                   Clear the screen.\n"
        "  dir [path]            List directory contents.\n"
        "  environ               Display environment variables.\n"
        "  echo [text]           Print text.\n"
        "  help                  Show help.\n"
        "  pause                 Pause shell.\n"
        "  quit                  Exit shell.\n"
        "  chmod <mode> <file>   Change permissions.\n"
        "  chown <owner> <file>  Change ownership.\n"
        "  ls [-a] [-A] [-l]     List directory contents.\n"
        "  pwd                   Print working directory.\n"
        "  cat <file>            Print file contents.\n"
        "  mkdir <dir>           Create directory.\n"
        "  rmdir <dir>           Remove directory.\n"
        "  rm [-r] [-f] <path>   Remove file or directory.\n"
        "  cp [-r] <src> <dst>   Copy.\n"
        "  mv <src> <dst>        Move.\n"
        "  touch <file>          Create empty file.\n"
        "  grep <pattern> <file> Search text.\n"
        "  wc [-l] [-w] [-c]     Count lines/words/chars.";

    return {0, out, ""};
}

/**
 * @brief Print all provided arguments separated by single spaces
 * @param args A vector of strings to print
 * @return Status code and the formatted output text
 */
CommandResult Commands::echo(const std::vector<std::string>& args) {
    std::string out;

    for (const std::string& arg : args) {
        out += arg + " ";
    }

    return {0, out, ""};
}

/**
 * @brief Pause execution of process
 * @param args Must be empty
 * @return Status code, empty output on success or error message on failure
 */
CommandResult Commands::pause(const std::vector<std::string> &args)
{
    if (!args.empty()){
        return {1, "", "pause: this command takes no arguments"};
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return {0, "", ""};
}

/**
 * @brief List the contents of the current working directory
 * @param args Optional flags:
 *        - "-a" include hidden entries
 *        - "-A" exclude "." and ".."
 *        - "-l" include detailed file information (permissions, access rights)
 * @return Status code, directory listing output, and possible error messages
 */
CommandResult Commands::ls(const std::vector<std::string>& args) {
    bool showAll = false;
    bool almostAll = false; 
    bool longList = false; 

    std::string out;
    std::string err;

    for (const std::string& arg : args) {
        if (arg == "-a") showAll = true;
        else if (arg == "-A") almostAll = true;
        else if (arg == "-l") longList = true;
        else return {1, "", "invalid flag -- '" + arg};
    }

    char cwd[FILENAME_MAX];
    if (!getcwd(cwd, FILENAME_MAX)) {
        return {1, "", "failed to get current directory"};
    }

    DIR* dirp = opendir(cwd);
    if (!dirp) {
        return {1, "", "cannot open directory: " + std::string(strerror(errno))};
    }

    struct dirent* dp;

    while ((dp = readdir(dirp)) != nullptr) {
        std::string name = dp->d_name;

        if (!showAll && !almostAll && name[0] == '.') continue;

        if (almostAll && (name == "." || name == "..")) continue;

        if (longList) {
            struct stat info;
            if (stat(name.c_str(), &info) == -1) {
                err += "cannot access " + name + ": " + strerror(errno);
                continue;
            }

            char type = S_ISDIR(info.st_mode) ? 'd' : '-';

            out += type;
            out += ((info.st_mode & S_IRUSR) ? 'r' : '-');
            out += ((info.st_mode & S_IWUSR) ? 'w' : '-');
            out += ((info.st_mode & S_IXUSR) ? 'x' : '-');
            out += ((info.st_mode & S_IRGRP) ? 'r' : '-');
            out += ((info.st_mode & S_IWGRP) ? 'w' : '-');
            out += ((info.st_mode & S_IXGRP) ? 'x' : '-');
            out += ((info.st_mode & S_IROTH) ? 'r' : '-');
            out += ((info.st_mode & S_IWOTH) ? 'w' : '-');
            out += ((info.st_mode & S_IXOTH) ? 'x' : '-');
            out += " " + std::to_string(info.st_size);
            out += " " + name + "\n";

            continue;
        }

        out += name + " ";
    }

    closedir(dirp);
    return {0, out, err};
}

/**
 * @brief Alias for ls command
 * @param args Refer to ls command
 * @return Status code, directory listing output, and possible error messages
 */
CommandResult Commands::dir(const std::vector<std::string>& args) {
    return ls(args);
}

/**
 * @brief Change the current working directory
 * @param args
 *        - zero arguments: go to the home directory
 *        - one argument: change to the specified path (supports ~ expansion)
 * @return Status code, empty output on success or error message on failure
 */
CommandResult Commands::cd(const std::vector<std::string>& args) {
    const char* home = getenv("HOME");

    if (args.empty()){
        if(chdir(home) == -1) {
            return {1, "", "cd: failed to change directory"};
        };
    } else if (args.size() == 1) {
        std::string path = args[0];

        if (path[0] == '~') {
            path = std::string(home) + path.substr(1);
        } 

        if(chdir(path.c_str()) == -1) {
            std::string errorMsg = "cd: failed to change directory: " + path;
            return {1, "", errorMsg};
        };

    } else {
        return {1, "", "cd: too many arguments"};
    }

    return {0, "", ""};
}