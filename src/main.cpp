#include <iostream>
#include <string>

int main() {
    while (true) {
        std::string input;

        std::cout << R"(
Testing Custom Shell. Enter any of the following commands, or -1 to quit:
- cd
- rmdir
- touch
- cp
- chown
- ls / dir
- echo
- grep
- help
- pause
- pwd
- mkdir
- rm
- mv
- chmod
- cat
- wc
- environ
- clr
- quit
)" << std::endl;

        std::cout << "Enter command: ";
        std::cin >> input;

        if (input == "-1") {
            std::cout << "Exiting shell..." << std::endl;
            break;
        }
    }

    return 0;
}
