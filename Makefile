# Define the compiler and compilation flags
CXX = g++
CXXFLAGS = -Wall -Wextra

# Define source files and their corresponding object files
SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:src/%.cpp=Bin/%.o)

# Define the executable name
EXEC = myprogram

# Define the directory for binaries
BINDIR = Bin

# Default target
all: $(EXEC)

# Compile C++ source files into object files in Bin directory
Bin/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ -Iinclude

# Link object files into the executable
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(BINDIR)/$(EXEC)

# Move the executable to the Bin directory
install: $(EXEC)
	mkdir -p $(BINDIR)
	mv $(EXEC) $(BINDIR)/$(EXEC)

# Clean up object files and the executable
clean:
	rm -f $(OBJS) $(BINDIR)/$(EXEC)

# Phony targets
.PHONY: all install clean
