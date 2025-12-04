// pac_character.cpp

#include "ipac_character.h"
#include <cmath>

// ============== PacCharacter Base Class Implementation ==============

/**
 * Constructor for the PacCharacter base class
 * @param row Row position of the character
 * @param col Column position of the character
 * @param appearance Character representation ('P' for Pacman, 'G' for Ghost)
 */
PacCharacter::PacCharacter(int row, int col, char appearance) 
    : row_(row), col_(col), appearance_(appearance) {}

/**
 * Get the row position of the character
 * @return Current row
 */
int PacCharacter::getRow() const {
    return row_;
}

/**
 * Get the column position of the character
 * @return Current column
 */
int PacCharacter::getCol() const {
    return col_;
}

/**
 * Get the appearance character
 * @return The character used to display this entity
 */
char PacCharacter::getAppearance() const {
    return appearance_;
}

/**
 * Set the position of the character
 * @param row New row position
 * @param col New column position
 */
void PacCharacter::setPosition(int row, int col) {
    row_ = row;
    col_ = col;
}

/**
 * Set the appearance character
 * @param appearance New appearance character
 */
void PacCharacter::setAppearance(char appearance) {
    appearance_ = appearance;
}

// ============== Pacman Class Implementation ==============

/**
 * Pacman constructor - initializes with 'P' appearance
 * @param row Starting row
 * @param col Starting column
 */
Pacman::Pacman(int row, int col) : PacCharacter(row, col, 'P') {}

/**
 * Pacman's movement is controlled by the user, not automatically
 * This override demonstrates POLYMORPHISM
 * @return Always returns STAY since user controls movement
 */
Direction Pacman::decideMove([[maybe_unused]] int targetRow, [[maybe_unused]] int targetCol) {
    // Pacman doesn't auto-move - controlled by user input
    return Direction::STAY;
}

// ============== Ghost Class Implementation ==============

/**
 * Ghost constructor - initializes with 'G' appearance
 * @param row Starting row
 * @param col Starting column
 */
Ghost::Ghost(int row, int col) : PacCharacter(row, col, 'G') {}

/**
 * Ghost AI movement - always moves toward Pacman
 * This override demonstrates POLYMORPHISM - different behavior than Pacman
 * @param targetRow Pacman's row position
 * @param targetCol Pacman's column position
 * @return Direction to move toward Pacman
 */
Direction Ghost::decideMove(int targetRow, int targetCol) {
    // Calculate distance to target
    int dx = std::abs(targetCol - col_);
    int dy = std::abs(targetRow - row_);

    // If already at target position, stay
    if (dx == 0 && dy == 0) {
        return Direction::STAY;
    }

    // If aligned on one axis, move along the other
    if (dx == 0 || dy == 0) {
        if (dx == 0 && targetRow > row_) {
            return Direction::DOWN;
        }
        else if (dx == 0 && targetRow < row_) {
            return Direction::UP;
        }
        else if (dy == 0 && targetCol > col_) {
            return Direction::RIGHT;
        }
        else if (dy == 0 && targetCol < col_) {
            return Direction::LEFT;
        }
    }

    // Move along the axis with greater distance
    // If dx > dy, prioritize vertical movement
    if (dx > dy) {
        if (targetRow > row_) {
            return Direction::DOWN;
        } else {
            return Direction::UP;
        }
    }
    // Otherwise, prioritize horizontal movement
    else {
        if (targetCol > col_) {
            return Direction::RIGHT;
        } else {
            return Direction::LEFT;
        }
    }

    return Direction::STAY;
}