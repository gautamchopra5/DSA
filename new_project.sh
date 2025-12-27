#!/usr/bin/env bash
set -euo pipefail

if [[ $# -ne 1 ]]; then
  echo "Usage: $0 <project-name>"
  exit 1
fi

project="$1"

if [[ -e "$project" ]]; then
  echo "Error: '$project' already exists."
  exit 1
fi

mkdir -p "$project"/{src,include}

cat > "$project/Makefile" <<'EOF'
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
EOF

cat > "$project/src/main.cpp" <<'EOF'
#include <iostream>

int main() {
    std::cout << "Hello from __PROJECT__!\n";
    return 0;
}
EOF

sed -i "s/__PROJECT__/${project}/g" "$project/src/main.cpp"

echo "Created project: $project"
