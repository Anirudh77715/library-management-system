CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
LDFLAGS = 

# Source files
SRCS = account.cpp book.cpp library.cpp main.cpp user.cpp utils.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Target executable
TARGET = library_system

# Default target
all: directories $(TARGET)

# Create data directory if it doesn't exist
directories:
	mkdir -p data

# Link object files to create executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJS) $(TARGET)

# Clean all, including data files (use with caution)
cleanall: clean
	rm -f data/*.txt

run: all
	./$(TARGET)

.PHONY: all clean cleanall run directories
