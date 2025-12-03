# OS-Shell-Clone

This project implements a simplified Unix command-line shell. It includes:
- A modular architecture (based on the REPL) with distinct components:
  - **Lexer**
  - **Parser**
  - **Abstract Syntax Tree (AST)**
  - **Executor**
  - **Command implementations**
- An **operator-precedence parser** using the **Precedence Climbing Method**
- Custom implementations of many core shell commands (`ls`, `grep`, `cp`, `rm`, `mkdir`, `chmod`, etc) using **POSIX System Calls**

## Installation

### Clone the repository:

```bash
git clone https://github.com/Romerico234/OS-Shell-Clone.git
cd OS-Shell-Clone
```

### Follow [INSTRUCTIONS.md](INSTRUCTIONS.md)

