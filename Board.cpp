// board.cpp

#include "Board.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

// Static member initialization
const std::string Board::IO_EXCEPTION = "I/O Exception!";

/**
 * Constructor - creates a new game board of given size
 * Places Pacman in center and ghosts in corners
 */
Board::Board(int size) : GRID_SIZE(size), score(0), gameOver(false) {
    // Initialize grid and visited arrays
    grid.resize(GRID_SIZE, std::vector<char>(GRID_SIZE, '*'));
    visited.resize(GRID_SIZE, std::vector<bool>(GRID_SIZE, false));

    // Create Pacman at center - using polymorphism (Pacman* stored as derived type)
    int center = GRID_SIZE / 2;
    pacman = new Pacman(center, center);
    visited[pacman->getRow()][pacman->getCol()] = true;

    // Create 4 ghosts at corners - using polymorphism
    ghosts.push_back(new Ghost(0, 0));
    ghosts.push_back(new Ghost(0, GRID_SIZE - 1));
    ghosts.push_back(new Ghost(GRID_SIZE - 1, GRID_SIZE - 1));
    ghosts.push_back(new Ghost(GRID_SIZE - 1, 0));

    // Initialize grid with characters
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (i == pacman->getCol() && j == pacman->getRow()) {
                grid[j][i] = pacman->getAppearance();
            }
            else if (i == ghosts[0]->getCol() && j == ghosts[0]->getRow()) {
                grid[j][i] = ghosts[0]->getAppearance();
            }
            else if (i == ghosts[1]->getCol() && j == ghosts[1]->getRow()) {
                grid[j][i] = ghosts[1]->getAppearance();
            }
            else if (i == ghosts[2]->getCol() && j == ghosts[2]->getRow()) {
                grid[j][i] = ghosts[2]->getAppearance();
            }
            else if (i == ghosts[3]->getCol() && j == ghosts[3]->getRow()) {
                grid[j][i] = ghosts[3]->getAppearance();
            }
            else {
                grid[j][i] = '*';
            }
        }
    }
}

/**
 * Constructor - loads a game board from a file
 */
Board::Board(const std::string& fileName) : GRID_SIZE(0), score(0), gameOver(false) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        throw std::runtime_error(IO_EXCEPTION);
    }

    // Read size
    int size;
    file >> size;
    const_cast<int&>(GRID_SIZE) = size;

    // Read score
    file >> score;
    file.ignore(); // Skip newline

    // Initialize arrays
    grid.resize(GRID_SIZE, std::vector<char>(GRID_SIZE));
    visited.resize(GRID_SIZE, std::vector<bool>(GRID_SIZE, false));

    // Read grid line by line
    std::string line;
    for (int r = 0; r < GRID_SIZE; r++) {
        std::getline(file, line);
        for (int c = 0; c < GRID_SIZE && c < static_cast<int>(line.length()); c++) {
            char ch = line[c];
            grid[r][c] = ch;

            if (ch == 'P') {
                pacman = new Pacman(r, c);
                visited[r][c] = true;
            }
            else if (ch == 'G') {
                ghosts.push_back(new Ghost(r, c));
            }
            else if (ch == ' ') {
                visited[r][c] = true;
            }
        }
    }
    file.close();
}

/**
 * Destructor - clean up dynamically allocated memory
 */
Board::~Board() {
    delete pacman;
    for (Ghost* ghost : ghosts) {
        delete ghost;
    }
    ghosts.clear();
}

/**
 * Save the current board state to a file
 */
void Board::saveBoard(const std::string& outputFile) {
    std::ofstream file(outputFile);
    if (!file.is_open()) {
        throw std::runtime_error(IO_EXCEPTION);
    }

    file << GRID_SIZE << std::endl;
    file << score << std::endl;
    
    for (int r = 0; r < GRID_SIZE; r++) {
        for (int c = 0; c < GRID_SIZE; c++) {
            file << grid[r][c];
        }
        file << std::endl;
    }
    file.close();
}

/**
 * Mark a position as visited and add points
 */
void Board::setVisited(int x, int y) {
    visited[x][y] = true;
    score += 10;
}

/**
 * Refresh the grid to show current positions
 */
void Board::refreshGrid() {
    int p_x = pacman->getCol();
    int p_y = pacman->getRow();
    int g0_x = ghosts[0]->getCol(), g0_y = ghosts[0]->getRow();
    int g1_x = ghosts[1]->getCol(), g1_y = ghosts[1]->getRow();
    int g2_x = ghosts[2]->getCol(), g2_y = ghosts[2]->getRow();
    int g3_x = ghosts[3]->getCol(), g3_y = ghosts[3]->getRow();

    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (i == p_x && j == p_y) {
                grid[j][i] = pacman->getAppearance();
            }
            else if (i == g0_x && j == g0_y) {
                grid[j][i] = ghosts[0]->getAppearance();
            }
            else if (i == g1_x && j == g1_y) {
                grid[j][i] = ghosts[1]->getAppearance();
            }
            else if (i == g2_x && j == g2_y) {
                grid[j][i] = ghosts[2]->getAppearance();
            }
            else if (i == g3_x && j == g3_y) {
                grid[j][i] = ghosts[3]->getAppearance();
            }
            else {
                if (visited[j][i]) {
                    grid[j][i] = ' ';
                } else {
                    grid[j][i] = '*';
                }
            }
        }
    }
}

/**
 * Check if Pacman can move in the given direction
 */
bool Board::canMove(Direction direction) {
    int x = pacman->getCol() + DirectionHelper::getX(direction);
    int y = pacman->getRow() + DirectionHelper::getY(direction);
    
    return (x >= 0 && x < GRID_SIZE) && (y >= 0 && y < GRID_SIZE);
}

/**
 * Move Pacman and update all ghosts
 */
void Board::move(Direction direction) {
    int p_x = pacman->getCol();
    int p_y = pacman->getRow();
    int x = DirectionHelper::getX(direction);
    int y = DirectionHelper::getY(direction);

    std::cout << "Starting at: " << p_x << "," << p_y << std::endl;
    std::cout << "Moving in: " << x << "," << y << std::endl;
    std::cout << "Moving to: " << (p_x + x) << "," << (p_y + y) << std::endl;

    if (canMove(direction)) {
        pacman->setPosition(p_y + y, p_x + x);
        if (!visited[p_y + y][p_x + x]) {
            setVisited(p_y + y, p_x + x);
        }
    }

    // Move each ghost using POLYMORPHISM
    // The ghostMove function uses the ghost's decideMove method
    for (int i = 0; i < G_NUM; i++) {
        Direction ghostDir = ghostMove(ghosts[i]);
        int g_x = ghosts[i]->getCol() + DirectionHelper::getX(ghostDir);
        int g_y = ghosts[i]->getRow() + DirectionHelper::getY(ghostDir);
        ghosts[i]->setPosition(g_y, g_x);
        isGameOver();
    }

    refreshGrid();
}

/**
 * Check if game is over (Pacman caught)
 */
bool Board::isGameOver() {
    int p_x = pacman->getRow();
    int p_y = pacman->getCol();
    int g0_x = ghosts[0]->getRow(), g0_y = ghosts[0]->getCol();
    int g1_x = ghosts[1]->getRow(), g1_y = ghosts[1]->getCol();
    int g2_x = ghosts[2]->getRow(), g2_y = ghosts[2]->getCol();
    int g3_x = ghosts[3]->getRow(), g3_y = ghosts[3]->getCol();

    if ((p_x == g0_x && p_y == g0_y) || (p_x == g1_x && p_y == g1_y) ||
        (p_x == g2_x && p_y == g2_y) || (p_x == g3_x && p_y == g3_y)) {
        pacman->setAppearance('X');
        gameOver = true;
        return true;
    }
    return false;
}

/**
 * Determine ghost movement direction toward Pacman
 * Uses POLYMORPHISM - calls the ghost's decideMove method
 */
Direction Board::ghostMove(PacCharacter* ghost) {
    if (isGameOver()) {
        pacman->setAppearance('X');
        return Direction::STAY;
    }

    // Use polymorphism - call the ghost's decideMove method
    Direction result = ghost->decideMove(pacman->getRow(), pacman->getCol());
    
    refreshGrid();
    return result;
}

/**
 * Convert board to string for display
 */
std::string Board::toString() const {
    std::ostringstream oss;
    oss << "Score: " << score << std::endl;
    oss << toStringHelper();
    return oss.str();
}

/**
 * Helper function - converts grid to string
 */
std::string Board::toStringHelper() const {
    std::string str = "";
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            str += "  ";
            str += grid[i][j];
        }
        str += "\n";
    }
    return str;
}

// Getters
int Board::getScore() const {
    return score;
}

bool Board::getIsGameOver() const {
    return gameOver;
}

int Board::getGridSize() const{
    return GRID_SIZE;
}

char Board::getGridChar(int row, int col) const {
    if (row >= 0 && row < GRID_SIZE && col >= 0 && col < GRID_SIZE) {
        return grid[row][col];
    }
    return ' ';
}

void Board::setGridChar(int row, int col, char ch) {
    if (row >= 0 && row < GRID_SIZE && col >= 0 && col < GRID_SIZE) {
        grid[row][col] = ch;
    }
}

const std::vector<std::vector<char>>& Board::getGrid() const {
    return grid;
}