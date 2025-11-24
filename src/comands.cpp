#include "commands.h"
#include <iostream>
#include <limits>
#include <string>
#include <dirent.h>
#include <unistd.h>

int Commands::help(const std::vector<std::string>& args) {
    std::cout <<
    "Available Commands:\n\n"
    "  cd [dir]              Change directory. If no argument is given, prints the current directory.\n"
    "\n"
    "  clr                   Clear the screen.\n"
    "\n"
    "  dir [path]            List files and subdirectories in the given directory.\n"
    "                        Defaults to the current directory.\n"
    "\n"
    "  environ               Display all environment variables.\n"
    "\n"
    "  echo [text]           Print text to standard output.\n"
    "                        Multiple spaces/tabs are collapsed.\n"
    "\n"
    "  help                  Display this help manual.\n"
    "\n"
    "  pause                 Pause shell execution until Enter is pressed.\n"
    "\n"
    "  quit                  Exit the shell.\n"
    "\n"
    "  chmod <mode> <file>   Change file permissions (e.g., chmod 755 file.txt).\n"
    "\n"
    "  chown <owner>[:group] <file>\n"
    "                        Change file ownership.\n"
    "\n"
    "  ls [-a] [-l] [path]   List directory contents.\n"
    "                        -a : show hidden files\n"
    "                        -l : long listing format\n"
    "\n"
    "  dir [-a] [-l] [path]   List directory contents.\n"
    "                        -a : show hidden files\n"
    "                        -l : long listing format\n"
    "\n"
    "  pwd                   Print the current working directory.\n"
    "\n"
    "  cat <file...>         Display the contents of one or more files.\n"
    "\n"
    "  mkdir <dir...>        Create one or more directories.\n"
    "\n"
    "  rmdir <dir...>        Remove empty directories.\n"
    "\n"
    "  rm [-r] [-f] <path>   Delete files or directories.\n"
    "                        -r : recursive\n"
    "                        -f : force\n"
    "\n"
    "  cp [-r] <src> <dst>   Copy files or directories.\n"
    "                        -r : recursive\n"
    "\n"
    "  mv <src> <dst>        Move or rename files or directories.\n"
    "\n"
    "  touch <file...>       Create empty files or update timestamps.\n"
    "\n"
    "  grep [-i] [-n] [-v] <pattern> <file>\n"
    "                        Search for patterns in files.\n"
    "                        -i : case-insensitive\n"
    "                        -n : show line numbers\n"
    "                        -v : invert match\n"
    "\n"
    "  wc [-l] [-w] [-c] <file>\n"
    "                        Count lines, words, or characters.\n"
    "                        -l : lines\n"
    "                        -w : words\n"
    "                        -c : characters\n"
    << std::endl;
    return 0;
}

int Commands::echo(const std::vector<std::string>& args) {
    for (const std::string& arg : args) {
        std::cout << arg << " ";
    }
    std::cout << std::endl;
    return 0;
}

int Commands::pause(const std::vector<std::string>& args) {
    if (!args.empty()) {
        std::cerr << "pause: this command takes no arguments" << std::endl;
        return 1;
    }
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return 0;
}

int Commands::ls(const std::vector<std::string>& args) {
    return 0;
}

int Commands::dir(const std::vector<std::string>& args) {
    return 0;
}

int Commands::cd(const std::vector<std::string>& args) {
    return 0;
}