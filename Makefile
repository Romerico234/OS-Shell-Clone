CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -Iinclude
SRC := $(wildcard src/*.cpp)
BIN := bin/myshell

all: $(BIN)

$(BIN): $(SRC) | bin
	$(CXX) $(CXXFLAGS) $(SRC) -o $(BIN)

bin:
	mkdir -p bin

clean:
	rm -f $(BIN)

run: all
	./$(BIN)
