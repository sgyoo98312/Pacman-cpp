// main.cpp
// Entry point for Pac-Man game (Terminal and GUI versions)

#include <iostream>
#include <string>
#include <vector>
#include "game_pacman.h"

#define GUI_ENABLED
#ifdef GUI_ENABLED
#include "gui_pacman.h"   
#endif

const int DEFAULT_SIZE = 10;
const std::string DEFAULT_FILE_NAME = "Pac-Man.board";

void printUsage() {
    std::cout << "Pac-Man" << std::endl;
    std::cout << "Usage: pacman [-g] [-s size] [-i inputFile] [-o outputFile]" << std::endl;
    std::cout << std::endl;
    std::cout << "  -g         -> Run in GUI mode (requires SFML)" << std::endl;
    std::cout << "  -s [size]  -> Board size (default: 10)" << std::endl;
    std::cout << "  -i [file]  -> Load game from file" << std::endl;
    std::cout << "  -o [file]  -> Save game to file" << std::endl;
}

int main(int argc, char** argv) {
    std::vector<std::string> args(argv, argv + argc);
    
    std::string inputFile = "";
    std::string outputFile = DEFAULT_FILE_NAME;
    int boardSize = DEFAULT_SIZE;
    bool useGui = false;
    bool hasInput = false;
    
    // Parse arguments
    for (size_t i = 1; i < args.size(); i++) {
        if (args[i] == "-g") {
            useGui = true;
        }
        else if (args[i] == "-i" && i + 1 < args.size()) {
            inputFile = args[++i];
            hasInput = true;
        }
        else if (args[i] == "-o" && i + 1 < args.size()) {
            outputFile = args[++i];
        }
        else if (args[i] == "-s" && i + 1 < args.size()) {
            boardSize = std::stoi(args[++i]);
            if (boardSize < 3) boardSize = DEFAULT_SIZE;
        }
        else if (args[i] == "-h" || args[i] == "--help") {
            printUsage();
            return 0;
        }
    }
    
    try {
        if (useGui) {
#ifdef GUI_ENABLED
            GuiPacman* game;
            if (hasInput) {
                game = new GuiPacman(inputFile, outputFile);
            } else {
                game = new GuiPacman(boardSize, outputFile);
            }
            game->run();
            delete game;
#else
            std::cerr << "GUI mode not enabled. Recompile with GUI_ENABLED defined." << std::endl;
            std::cerr << "Running in terminal mode instead..." << std::endl;
            GamePacman game;
            game.play(argc, argv);
#endif
        } else {
            // Terminal mode
            GamePacman game;
            game.play(argc, argv);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}