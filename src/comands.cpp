#include "commands.h"
#include <limits>
#include <string>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <fcntl.h>

/**
 * @brief Display a list of all supported shell commands
 * @param args Must be empty
 * @return Status code and a help message on success
 */
CommandResult Commands::helpCommand(const std::vector<std::string>& args) {
    if (!args.empty()) {
        return {1, "", "help: this command takes no arguments"};
    }

    std::string out =
        "Available Commands:\n"
        "  cd [dir]                       Change directory.\n"
        "  clr                            Clear the screen.\n"
        "  dir [-a] [-A] [-l] [path]      List directory contents.\n"
        "  environ                        Display environment variables.\n"
        "  echo [text]                    Print text.\n"
        "  help                           Show help.\n"
        "  pause                          Pause shell.\n"
        "  quit                           Exit shell.\n"
        "  chmod <mode> <file>            Change permissions.\n"
        "  chown <owner> <file>           Change ownership.\n"
        "  ls [-a] [-A] [-l] [path]       List directory contents.\n"
        "  pwd                            Print working directory.\n"
        "  cat <file>                     Print file contents.\n"
        "  mkdir <dir>                    Create directory.\n"
        "  rmdir <dir>                    Remove directory.\n"
        "  rm [-r] [-f] <path>            Remove file or directory.\n"
        "  cp [-r] <src> <dst>            Copy.\n"
        "  mv <src> <dst>                 Move.\n"
        "  touch <file>                   Create empty file.\n"
        "  grep <pattern> <file>          Search text.\n"
        "  wc [-l] [-w] [-c]              Count lines/words/chars.";

    return {0, out, ""};
}

/**
 * @brief Print all provided arguments separated by single spaces
 * @param args A vector of strings to print
 * @return Status code and the formatted output text
 */
CommandResult Commands::echoCommand(const std::vector<std::string>& args) {
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
CommandResult Commands::pauseCommand(const std::vector<std::string> &args)
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
CommandResult Commands::lsCommand(const std::vector<std::string>& args) {
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
CommandResult Commands::dirCommand(const std::vector<std::string>& args) {
    return lsCommand(args);
}

/**
 * @brief Change the current working directory
 * @param args
 *        - zero arguments: go to the home directory
 *        - one argument: change to the specified path (supports ~ expansion)
 * @return Status code, empty output on success or error message on failure
 */
CommandResult Commands::cdCommand(const std::vector<std::string>& args) {
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

/**
 * @brief Deletes the specified directory
 * @param args
 *       - one argument: path to the directory to remove
 *       - two arguments: "-p" flag followed by the specified paths to remove
 * * @return Status code, directory deletion messages on success, or an error message on failure
 */
CommandResult Commands::rmdirCommand(const std::vector<std::string>& args) {
    if (args.empty()) {
        return {1, "", "rmdir: missing operand"};
    }

    if (args.size() == 2) {
        if (args[0] == "-p") {
            std::string path = args[1];
            std::string out = "";

            if (path.empty()) {
                return {1, "", "rmdir: no path specified"};
            }

            // Avoids possible infinite loops (because of trailing slashes)
            while (!path.empty() && path.back() == '/') {
                path.pop_back();
            }

            while (!path.empty()) {

                if (rmdir(path.c_str()) == -1) {
                    return {1, "", formatRmdirErrorMsg(path)};
                }

                size_t pos = path.find_last_of('/');
                if (pos == std::string::npos) break;

                path = path.substr(0, pos);

                // Avoids deleting the root directory
                if (path == "" || path == "/") break;
            }

            return {0, "", ""};
        }
        else {
            return {1, "", "rmdir: unrecognized option '" + args[0] + "'"};
        }
    }

    if (args.size() > 2) {
        return {1, "", "rmdir: too many arguments"};
    }

    std::string path = args[0];

    if (rmdir(path.c_str()) == -1) {
        return {1, "", formatRmdirErrorMsg(path)};
    }

    return {0, ""};
}

/**
 * @brief Creates a file if it does not exit or updates an existing file's access/modification times
 * @param args The file name
 * @return status code, empty output on success or error message on failure
 */
CommandResult Commands::touchCommand(const std::vector<std::string>& args) {
    /**
     * TODO: Implement optional flags
     *        - "-a"
     *        - "-m"
     *        - "-t"
     *        - "-c"
    */

    if (args.empty() || args.size() > 1) {
        return {1, "", "touch: invalid arguments passed"};
    }

    std::string fileName = args[0];
    
    /**
     * @note O_CREAT flag creates the file if it does not exist
     * @note 0644 is the permission bits that allows read and write for the owner, and read-only for group and the public
    */
    int fd = open(fileName.c_str(), O_CREAT | O_WRONLY, 0644);

    if (fd == -1) {
        return {1, "", "touch: cannot create file '" + fileName + "': " + std::string(strerror(errno))};
    }

    close(fd); 

    return {0, "", ""};
}

/**
 * @brief 
 * @param args
 * @return 
 */
CommandResult Commands::cpCommand(const std::vector<std::string>& args) {
    return {0, "", ""};
}

/**
 * @brief 
 * @param args
 * @return 
 */
CommandResult Commands::chownCommand(const std::vector<std::string>& args) {
    return {0, "", ""};
}

/**
 * @brief 
 * @param args
 * @return 
 */
CommandResult Commands::grepCommand(const std::vector<std::string>& args) {
    return {0, "", ""};
}

/* --- Helper Functions --- */
std::string Commands::formatRmdirErrorMsg(const std::string& path) {
    switch (errno) {
        case ENOTEMPTY:
        case EEXIST:
            return "rmdir: failed to remove '" + path + "': directory not empty";

        case ENOENT:
            return "rmdir: failed to remove '" + path + "': no such file or directory";

        case ENOTDIR:
            return "rmdir: failed to remove '" + path + "': not a directory";

        case EACCES:
        case EPERM:
            return "rmdir: failed to remove '" + path + "': permission denied";

        default:
            return "rmdir: failed to remove '" + path + "': " + std::string(strerror(errno));
    }
}
