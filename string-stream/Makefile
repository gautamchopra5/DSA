CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -g
INCLUDES := -Iinclude
SRC := $(wildcard src/*.cpp)
BIN := bin/main

.PHONY: all run clean

all: $(BIN)

$(BIN): $(SRC) | bin
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SRC) -o $(BIN)

bin:
	mkdir -p bin

run: $(BIN)
	./$(BIN)

clean:
	rm -rf bin
