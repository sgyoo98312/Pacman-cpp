# Makefile for Pac-Man Game

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -I/opt/homebrew/opt/sfml@2/include
SFML_LIBS = -L/opt/homebrew/opt/sfml@2/lib -lsfml-graphics -lsfml-window -lsfml-system

# Source files
SRCS = main.cpp game_pacman.cpp game_manager.cpp Board.cpp pac_character.cpp gui_pacman.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable name
TARGET = pacman

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(SFML_LIBS)

# Compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJS) $(TARGET)

# Run the game (terminal mode)
run: $(TARGET)
	./$(TARGET)

# Run in GUI mode
run-gui: $(TARGET)
	./$(TARGET) -g

.PHONY: all clean run run-gui