# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wpedantic -O2

# Google Test and Google Benchmark flags
GTEST_LIBS = -lgtest -lgtest_main
GBENCH_LIBS = -lbenchmark -lbenchmark_main

# Source files
SRC_MAIN = main.cc
SRC_TEST = test.cc
SRC_BENCH = bench.cc

# Output files
EXE_MAIN = main
EXE_TEST = test
EXE_BENCH = bench

# Targets
all: $(EXE_MAIN) $(EXE_TEST) $(EXE_BENCH) bench-san

$(EXE_MAIN): $(SRC_MAIN)
	$(CXX) $(CXXFLAGS) -o $@ $^ -lpthread -fsanitize=thread

$(EXE_TEST): $(SRC_TEST)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(GTEST_LIBS)

$(EXE_BENCH): $(SRC_BENCH)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(GBENCH_LIBS) -lpthread

bench-san: $(SRC_BENCH)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(GBENCH_LIBS) -lpthread -fsanitize=thread -g

# Clean up
clean:
	rm -f $(EXE_MAIN) $(EXE_TEST) $(EXE_BENCH) bench-san

.PHONY: all clean

