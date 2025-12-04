# Makefile for Pac-Man Game

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g

# Source files
SRCS = main.cpp game_pacman.cpp game_manager.cpp board.cpp pac_character.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable name
TARGET = pacman

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Dependencies
main.o: main.cpp game_pacman.h
game_pacman.o: game_pacman.cpp game_pacman.h game_manager.h
game_manager.o: game_manager.cpp game_manager.h board.h direction.h
board.o: board.cpp board.h ipac_character.h direction.h
pac_character.o: pac_character.cpp ipac_character.h direction.h

# Clean up
clean:
	rm -f $(OBJS) $(TARGET)

# Run the game
run: $(TARGET)
	./$(TARGET)

# Run with custom size
run-small: $(TARGET)
	./$(TARGET) -s 5

.PHONY: all clean run run-small