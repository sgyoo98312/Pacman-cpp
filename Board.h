// Board.h

#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <string>
#include <memory>
#include "ipac_character.h"
#include "Direction.h"

/**
 * Board class - manages the Pac-Man game board
 * Contains the grid, characters, score, and game logic
 */
class Board {
public:
    // Exception message for I/O errors
    static const std::string IO_EXCEPTION;

    /**
     * Constructor - creates a new game board of given size
     * @param size The size of the square board
     */
    Board(int size);

    /**
     * Constructor - loads a game board from a file
     * @param fileName The file to load from
     */
    Board(const std::string& fileName);

    /**
     * Destructor - cleans up dynamically allocated memory
     */
    ~Board();

    /**
     * Save the current board state to a file
     * @param outputFile The file to save to
     */
    void saveBoard(const std::string& outputFile);

    /**
     * Mark a position as visited and add to score
     * @param x Row position
     * @param y Column position
     */
    void setVisited(int x, int y);

    /**
     * Refresh the grid to reflect current character positions
     */
    void refreshGrid();

    /**
     * Check if Pacman can move in the given direction
     * @param direction The direction to check
     * @return true if move is valid, false otherwise
     */
    bool canMove(Direction direction);

    /**
     * Move Pacman in the given direction and update ghosts
     * @param direction The direction to move Pacman
     */
    void move(Direction direction);

    /**
     * Check if the game is over (Pacman caught by ghost)
     * @return true if game over, false otherwise
     */
    bool isGameOver();

    /**
     * Get the direction a ghost should move
     * @param ghost Pointer to the ghost character
     * @return The direction for the ghost to move
     */
    Direction ghostMove(PacCharacter* ghost);

    /**
     * Convert board to string representation
     * @return String showing score and board state
     */
    std::string toString() const;

    // Getters
    int getScore() const;
    bool getIsGameOver() const;
    
    /**
     * Get the grid size
     * @return The size of the grid
     */
    int getGridSize() const;
    
    /**
     * Get a character from the grid at specified position
     * @param row Row position
     * @param col Column position
     * @return The character at that position
     */
    char getGridChar(int row, int col) const;
    
    /**
     * Set a character in the grid at specified position
     * @param row Row position
     * @param col Column position
     * @param ch The character to set
     */
    void setGridChar(int row, int col, char ch);
    
    /**
     * Get the entire grid (for GUI rendering)
     * @return Reference to the grid
     */
    const std::vector<std::vector<char>>& getGrid() const;

private:
    const int GRID_SIZE;
    static const int G_NUM = 4;  // Number of ghosts

    std::vector<std::vector<char>> grid;      // Board representation
    std::vector<std::vector<bool>> visited;   // Track visited positions

    Pacman* pacman;                           // Pointer to Pacman (polymorphism)
    std::vector<Ghost*> ghosts;               // Pointers to ghosts (polymorphism)

    int score;
    bool gameOver;

    /**
     * Helper function for toString
     * @return Grid as string
     */
    std::string toStringHelper() const;
};

#endif // BOARD_H