# Compiler
CXX = g++
# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++11
# Define the directory for binaries
BINDIR = Bin
# Define source files and their corresponding object files
SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:src/%.cpp=$(BINDIR)/%.o)
# Define the executable name
EXEC = myprogram

# Default target
all: $(BINDIR)/$(EXEC)

# Compile C++ source files into object files in Bin directory
$(BINDIR)/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ -Iinclude

# Link object files into the executable
$(BINDIR)/$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Create the object files directory if it doesn't exist
$(shell mkdir -p $(BINDIR))

# Clean up object files and the executable
clean:
	rm -f $(OBJS) $(BINDIR)/$(EXEC)

# Phony targets
.PHONY: all clean
