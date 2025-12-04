// game_pacman.cpp

#include "game_pacman.h"

/**
 * Constructor
 */
GamePacman::GamePacman() : boardSize(DEFAULT_SIZE), gm(nullptr) {}

/**
 * Start the game
 */
void GamePacman::play(int argc, char** argv) {
    std::vector<std::string> args(argv, argv + argc);
    processArgs(args);
    
    if (gm != nullptr) {
        gm->play();
        delete gm;
        gm = nullptr;
    }
}

/**
 * Process command line arguments
 */
void GamePacman::processArgs(const std::vector<std::string>& args) {
    bool hasI = false, hasO = false, hasS = false;
    int iNum = 0, oNum = 0, sNum = 0;

    for (size_t a = 0; a < args.size(); a++) {
        if (args[a] == "-i") {
            hasI = true;
            iNum = a;
        }
        else if (args[a] == "-o") {
            hasO = true;
            oNum = a;
        }
        else if (args[a] == "-s") {
            hasS = true;
            sNum = a;
        }
    }

    if (hasI && hasO) {
        inputName = args[iNum + 1];
        outputName = args[oNum + 1];
        gm = new GameManager(inputName, outputName);
    }
    else if (hasI && !hasO) {
        inputName = args[iNum + 1];
        outputName = DEFAULT_FILE_NAME;
        gm = new GameManager(inputName, outputName);
    }
    else if (!hasI && hasO && hasS) {
        outputName = args[oNum + 1];
        boardSize = std::stoi(args[sNum + 1]);
        gm = new GameManager(boardSize, outputName);
    }
    else if (!hasI && !hasO && hasS) {
        boardSize = std::stoi(args[sNum + 1]);
        outputName = DEFAULT_FILE_NAME;
        gm = new GameManager(boardSize, outputName);
    }
    else if (!hasI && hasO && !hasS) {
        boardSize = DEFAULT_SIZE;
        outputName = args[oNum + 1];
        gm = new GameManager(boardSize, outputName);
    }
    else {
        outputName = DEFAULT_FILE_NAME;
        boardSize = DEFAULT_SIZE;
        gm = new GameManager(boardSize, outputName);
    }
}

void GamePacman::printUsage() {
    std::cout << "Pac-Man" << std::endl;
    std::cout << "Usage: GamePacman [-s size] [-i inputFile] [-o outputFile]" << std::endl;
}