CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -O2
CPPFLAGS = -Iinclude
TARGET = git-stats
SRCS = src/main.cpp src/git_root.cpp src/git_stats.cpp
OBJS = $(SRCS:.cpp=.o)

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

src/%.o: src/%.cpp include/git_stats.hpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

test: $(TARGET)
	python3 tests/cli_tests.py || python tests/cli_tests.py

clean:
	rm -f $(OBJS) $(TARGET) $(TARGET).exe
