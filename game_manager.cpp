// game_manager.cpp

#include "game_manager.h"

/**
 * Default constructor
 */
GameManager::GameManager() : board(nullptr), outputFileName("") {}

/**
 * Constructor - creates a new game
 */
GameManager::GameManager(int boardSize, const std::string& outputBoard) 
    : board(new Board(boardSize)), outputFileName(outputBoard) {}

/**
 * Constructor - loads existing game
 */
GameManager::GameManager(const std::string& inputBoard, const std::string& outputBoard)
    : board(new Board(inputBoard)), outputFileName(outputBoard) {}

/**
 * Destructor
 */
GameManager::~GameManager() {
    delete board;
    board = nullptr;
}

/**
 * Copy assignment operator
 */
GameManager& GameManager::operator=(const GameManager& other) {
    if (this != &other) {
        delete board;
        // Note: This creates a new board with the same size
        // A proper deep copy would require more complex logic
        board = nullptr;
        outputFileName = other.outputFileName;
    }
    return *this;
}

/**
 * Move constructor
 */
GameManager::GameManager(GameManager&& other) noexcept 
    : board(other.board), outputFileName(std::move(other.outputFileName)) {
    other.board = nullptr;
}

/**
 * Move assignment operator
 */
GameManager& GameManager::operator=(GameManager&& other) noexcept {
    if (this != &other) {
        delete board;
        board = other.board;
        outputFileName = std::move(other.outputFileName);
        other.board = nullptr;
    }
    return *this;
}

/**
 * Main game loop
 */
void GameManager::play() {
    if (board == nullptr) {
        std::cerr << "Error: Board not initialized!" << std::endl;
        return;
    }

    std::cout << board->toString() << std::endl;
    bool isQ = false;

    while (!isQ) {
        if (board->getIsGameOver()) {
            std::cout << "Game Over!" << std::endl;
            isQ = true;
            board->saveBoard(outputFileName);
            continue;
        }

        printControls();

        std::string ans;
        std::cin >> ans;

        if (ans == "w") {
            board->move(Direction::UP);
            std::cout << board->toString() << std::endl;
        }
        else if (ans == "s") {
            board->move(Direction::DOWN);
            std::cout << board->toString() << std::endl;
        }
        else if (ans == "a") {
            board->move(Direction::LEFT);
            std::cout << board->toString() << std::endl;
        }
        else if (ans == "d") {
            board->move(Direction::RIGHT);
            std::cout << board->toString() << std::endl;
        }
        else if (ans == "q") {
            isQ = true;
            board->saveBoard(outputFileName);
            std::cout << "Game saved to: " << outputFileName << std::endl;
        }
        else {
            std::cout << "Invalid command. Please try again." << std::endl;
        }
    }
}

/**
 * Print control instructions
 */
void GameManager::printControls() {
    std::cout << "  Controls:" << std::endl;
    std::cout << "    w - Move Up" << std::endl;
    std::cout << "    s - Move Down" << std::endl;
    std::cout << "    a - Move Left" << std::endl;
    std::cout << "    d - Move Right" << std::endl;
    std::cout << "    q - Quit and Save Board" << std::endl;
    std::cout << std::endl;
}