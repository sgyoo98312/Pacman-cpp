// game_pacman.h

#ifndef GAME_PACMAN_H
#define GAME_PACMAN_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include "game_manager.h"

/**
 * GamePacman class - entry point for the Pac-Man game
 * Handles command line arguments and starts the game
 */
class GamePacman {
public:
    /**
     * Constructor
     */
    GamePacman();

    /**
     * Start the game with command line arguments
     * @param argc Argument count
     * @param argv Argument values
     */
    void play(int argc, char** argv);

private:
    static const int DEFAULT_SIZE = 10;
    const std::string DEFAULT_FILE_NAME = "Pac-Man.board";

    int boardSize;
    std::string inputName;
    std::string outputName;
    GameManager* gm;

    /**
     * Process command line arguments
     * @param args Vector of argument strings
     */
    void processArgs(const std::vector<std::string>& args);

    /**
     * Print usage message
     */
    void printUsage();
};

#endif // GAME_PACMAN_H