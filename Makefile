# Compiler and flags
CXX = clang++
CXXFLAGS = -std=c++20 -Wall -Wpedantic -O2
CORO_FLAG = -fcoroutines # gcc
#CORO_FLAG = -stdlib=libc++ -fcoroutines # clang??

# Google Test and Google Benchmark flags
GTEST_LIBS = -lgtest -lgtest_main
GBENCH_LIBS = -lbenchmark -lbenchmark_main

# Source files
SRC_MAIN = main.cc
SRC_CORO = coro.cc
SRC_TEST = test.cc
SRC_BENCH = bench.cc

# Output files
EXE_MAIN = main
EXE_CORO = coro
EXE_TEST = test
EXE_BENCH = bench

# Targets
all: $(EXE_MAIN) $(EXE_TEST) $(EXE_BENCH) bench-san $(EXE_CORO)

$(EXE_MAIN): $(SRC_MAIN)
	$(CXX) $(CXXFLAGS) -o $@ $^ -lpthread -fsanitize=thread

$(EXE_CORO): $(SRC_CORO)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(CORO_FLAG)

$(EXE_TEST): $(SRC_TEST)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(GTEST_LIBS)

$(EXE_BENCH): $(SRC_BENCH)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(GBENCH_LIBS) -lpthread

bench-san: $(SRC_BENCH)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(GBENCH_LIBS) -lpthread -fsanitize=thread -g

# Clean up
clean:
	rm -f $(EXE_MAIN) $(EXE_TEST) $(EXE_BENCH) bench-san $(EXE_CORO)

.PHONY: all clean

