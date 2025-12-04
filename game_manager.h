// game_manager.h

#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <iostream>
#include <string>
#include "Board.h"
#include "Direction.h"

/**
 * GameManager class - handles user input and game loop
 * Allows users to control Pac-Man movement and save/load games
 */
class GameManager {
public:
    /**
     * Default constructor - needed for member initialization
     */
    GameManager();

    /**
     * Constructor - creates a new game with given board size
     * @param boardSize Size of the game board
     * @param outputBoard Filename to save the board to
     */
    GameManager(int boardSize, const std::string& outputBoard);

    /**
     * Constructor - loads an existing game from file
     * @param inputBoard Filename to load the board from
     * @param outputBoard Filename to save the board to
     */
    GameManager(const std::string& inputBoard, const std::string& outputBoard);

    /**
     * Destructor - cleans up board memory
     */
    ~GameManager();

    /**
     * Copy constructor - deleted to prevent copying
     */
    GameManager(const GameManager&) = delete;

    /**
     * Copy assignment - handles proper copying
     */
    GameManager& operator=(const GameManager& other);

    /**
     * Move constructor
     */
    GameManager(GameManager&& other) noexcept;

    /**
     * Move assignment
     */
    GameManager& operator=(GameManager&& other) noexcept;

    /**
     * Main game loop - takes user input and executes moves
     * Valid moves:
     *   w - Move Up
     *   s - Move Down
     *   a - Move Left
     *   d - Move Right
     *   q - Quit and Save Board
     */
    void play();

private:
    Board* board;              // The game board (pointer for polymorphism)
    std::string outputFileName; // File to save board to when exiting

    /**
     * Print control instructions
     */
    void printControls();
};

#endif // GAME_MANAGER_H