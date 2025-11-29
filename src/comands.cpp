#include "commands.h"
#include <limits>
#include <string>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <limits.h>
#include <vector>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>

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

CommandResult Commands::quit(const std::vector<std::string>& args) 
{
    if (!args.empty()) {
        return {1, "", "quit: this command takes no arguments"};
    }

    std::cout << "Exiting Shell...\n";
    std::exit(0);
}


CommandResult Commands::clr(const std::vector<std::string>& args) 
{
    if (!args.empty()) {
        return {1, "", "clr: this command takes no arguments"};
    }

    std::cout << "\033[H\033[J"; //ANSI escape code which works on most terminals. It escapes with \033, then [H moves the cursor to the home position, and [J clears the screen from the cursor down.
    return {0, "", ""};
}

CommandResult Commands::pwd(const std::vector<std::string>& args) 
{
    if (!args.empty()) {
        return {1, "", "pwd: this command takes no arguments"};
    }

    char cwd[PATH_MAX];
    if (!getcwd(cwd, sizeof(cwd))) {
        return {1, "", "pwd: failed to get current directory"};
    }

    return {0, std::string(cwd), ""};
}

CommandResult Commands::environ(const std::vector<std::string>& args) 
{
    if (!args.empty()) {
        return {1, "", "environ: this command takes no arguments"};
    }

    std::string out;

    // Use ::environ to avoid name conflict with Commands::environ
    for (char **env = ::environ; *env != nullptr; env++) {
        out += std::string(*env) + "\n";
    }

    return {0, out, ""};
}

CommandResult Commands::cat(const std::vector<std::string>& args) 
{
    if (args.empty()) {
        return {1, "", "cat: missing file operand"};
    }

    std::string out;
    std::string err;
    const size_t bufferSize = 4096; // an average buffer size for reading files or just increase it if needed.
    char buffer[bufferSize];

    for (const std::string& filename : args) {
        int fd = open(filename.c_str(), O_RDONLY); // to open the file and return file descriptor which is the number the OS uses to refer to the open file.
        if (fd == -1) {
            err += "cat: cannot open " + filename + ": " + strerror(errno) + "\n";
            continue;
        }

        ssize_t bytesRead;
        while ((bytesRead = read(fd, buffer, bufferSize)) > 0) { //reads buffer size and loops until there is nothing left to read.
            out.append(buffer, bytesRead);  
        }

        if (bytesRead == -1) {
            err += "cat: error reading " + filename + ": " + strerror(errno) + "\n";
        }

        close(fd);
    }

    return {err.empty() ? 0 : 1, out, err};
}

CommandResult Commands::wc(const std::vector<std::string>& args) 
{
    bool countLines = false;
    bool countWords = false;
    bool countChars = false;
    std::vector<std::string> files;

    // Parse arguments
    for (const std::string& arg : args) {
        if (arg == "-l") countLines = true;
        else if (arg == "-w") countWords = true;
        else if (arg == "-c") countChars = true;
        else files.push_back(arg);
    }

    // If no flags specified, count all
    if (!countLines && !countWords && !countChars) 
    {
        countLines = countWords = countChars = true;
    }

    if (files.empty()) {
        return {1, "", "wc: missing file operand"};
    }

    std::string out;
    std::string err;

    for (const std::string& filename : files) 
    {
        int fd = open(filename.c_str(), O_RDONLY);
        if (fd == -1) {
            err += "wc: cannot open file '" + filename + "': " + strerror(errno) + "\n";
            continue;
        }

        size_t lines = 0, words = 0, chars = 0;
        char buf[4096];
        bool inWord = false;
        ssize_t bytesRead;

        while ((bytesRead = read(fd, buf, sizeof(buf))) > 0) {
            chars += bytesRead;
            for (ssize_t i = 0; i < bytesRead; ++i) {
                char c = buf[i];
                if (c == '\n') lines++;
                if (isspace(c)) {
                    inWord = false;
                } else if (!inWord) {
                    words++;
                    inWord = true;
                }
            }
        }

        if (bytesRead == -1) {
            err += "wc: error reading file '" + filename + "': " + strerror(errno) + "\n";
        }

        close(fd);

        // Format output according to flags
        if (countLines) out += std::to_string(lines) + " ";
        if (countWords) out += std::to_string(words) + " ";
        if (countChars) out += std::to_string(chars) + " ";
        out += filename + "\n";
    }

    return {0, out, err};
}

CommandResult Commands::mkdir(const std::vector<std::string>& args) 
{
    if (args.empty()) {
        return {1, "", "mkdir: missing operand"};
    }

    std::string out;
    std::string err;

    for (const std::string& dir : args) {
        if (::mkdir(dir.c_str(), 0755) == -1) { //0755 is a common permission setting for directories. 7 means read, write, and execute permissions for the owner, 5 means read and execute permissions for the group, and 5 means read and execute permissions for others.
            err += "mkdir: cannot create directory '" + dir + "': " + strerror(errno) + "\n";
        }
    }

    return {0, out, err};
}

CommandResult Commands::rm(const std::vector<std::string>& args) {
    if (args.empty()) {
        return {1, "", "rm: missing operand"};
    }

    bool recursive = false;
    bool force = false;
    size_t currentArg = 0;

    // Parse flags like -r and -f
    while (currentArg < args.size() && args[currentArg][0] == '-') {
        const std::string& flag = args[currentArg];
        if (flag.find('r') != std::string::npos) recursive = true;
        if (flag.find('f') != std::string::npos) force = true;
        currentArg++;
        if (currentArg == args.size()) {
            return {1, "", "rm: missing operand after '" + flag + "'"};
        }
    }

    std::string output;
    std::string errors;

    // Loop through each file/directory argument
    for (; currentArg < args.size(); ++currentArg) {
        const std::string& path = args[currentArg];

        struct stat st;
        if (stat(path.c_str(), &st) == -1) {
            if (!force) {
                errors += "rm: cannot access '" + path + "': " + strerror(errno) + "\n";
            }
            continue;
        }

        // If it's a directory
        if (S_ISDIR(st.st_mode)) {
            if (recursive) {
                DIR* dir = opendir(path.c_str());
                if (!dir) {
                    if (!force) {
                        errors += "rm: cannot open directory '" + path + "': " + strerror(errno) + "\n";
                    }
                    continue;
                }

                // Recursively remove contents
                struct dirent* entry;
                while ((entry = readdir(dir)) != nullptr) {
                    std::string name = entry->d_name;
                    if (name == "." || name == "..") continue;
                    std::string subpath = path + "/" + name;
                    rm({recursive ? "-r" : "", force ? "-f" : "", subpath});
                }
                closedir(dir);

                // Remove the empty directory itself
                if (::rmdir(path.c_str()) == -1 && !force) {
                    errors += "rm: failed to remove directory '" + path + "': " + strerror(errno) + "\n";
                }
            } else {
                if (!force) errors += "rm: '" + path + "' is a directory\n";
            }
        } 
        // If it's a regular file
        else {
            if (::unlink(path.c_str()) == -1 && !force) {
                errors += "rm: cannot remove '" + path + "': " + strerror(errno) + "\n";
            }
        }
    }

    return {errors.empty() ? 0 : 1, output, errors};
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

                out += "Removed directory: " + path + "\n";

                size_t pos = path.find_last_of('/');
                if (pos == std::string::npos) break;

                path = path.substr(0, pos);

                // Avoids deleting the root directory
                if (path == "" || path == "/") break;
            }

            return {0, out, ""};
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

    return {0, "Removed directory: " + path, ""};
}

/**
 * @brief 
 * @param args
 * @return 
 */
CommandResult Commands::touchCommand(const std::vector<std::string>& args) {
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
