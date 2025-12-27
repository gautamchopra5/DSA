CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -g
INCLUDES := -Iinclude
BIN_DIR := bin

ifdef FILE
SRC := $(FILE)
BIN := $(BIN_DIR)/main
else
SRC := $(wildcard src/*.cpp)
BINS := $(patsubst src/%.cpp,$(BIN_DIR)/%,$(SRC))
endif

.PHONY: all run clean

ifdef FILE
all: $(BIN)
else
all: $(BINS)
endif

ifdef FILE
$(BIN): $(SRC) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SRC) -o $(BIN)
else
$(BIN_DIR)/%: src/%.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $< -o $@
endif

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

run: all
ifdef FILE
	./$(BIN)
else
	@echo "Set FILE=/path/to/file.cpp to run a specific day." >&2
	@exit 1
endif

clean:
	rm -rf $(BIN_DIR)
