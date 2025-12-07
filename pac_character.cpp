// pac_character.cpp
// Implementation of PacCharacter (base), Pacman (derived), and Ghost (derived)
// Demonstrates INHERITANCE, CONSTRUCTOR INITIALIZATION LISTS, and POLYMORPHISM

#include "ipac_character.h"
#include <cstdlib>  // for abs()

// ============================================================================
// BASE CLASS: PacCharacter Implementation
// ============================================================================

/**
 * Base class constructor
 * Uses INITIALIZATION LIST to set member variables (slide 2 of your notes)
 */
PacCharacter::PacCharacter(int row, int col, char appearance) 
    : row_(row), col_(col), appearance_(appearance) 
{
    // Initialization list is preferred over assignment in body
    // (See slide 2 of your inheritance notes)
}

/**
 * Virtual destructor
 * Required when using polymorphism to ensure proper cleanup
 */
PacCharacter::~PacCharacter() {
    // Base class destructor
}

/**
 * Getter for row position
 */
int PacCharacter::getRow() const {
    return row_;
}

/**
 * Getter for column position  
 */
int PacCharacter::getCol() const {
    return col_;
}

/**
 * Getter for character appearance
 */
char PacCharacter::getAppearance() const {
    return appearance_;
}

/**
 * Setter for position
 */
void PacCharacter::setPosition(int row, int col) {
    row_ = row;
    col_ = col;
}

/**
 * Setter for appearance
 */
void PacCharacter::setAppearance(char appearance) {
    appearance_ = appearance;
}


// ============================================================================
// DERIVED CLASS: Pacman Implementation
// ============================================================================

/**
 * Pacman constructor
 * 
 * IMPORTANT: Uses initialization list to call BASE CLASS CONSTRUCTOR
 * (See slide 22 of your inheritance notes)
 * 
 * Constructor call order: PacCharacter() -> Pacman()
 * (See slide 23 of your inheritance notes)
 */
Pacman::Pacman(int row, int col) 
    : PacCharacter(row, col, 'P')  // Call base class constructor with 'P' for Pacman
{
    // Pacman-specific initialization (if any) goes here
}

/**
 * Pacman destructor
 * 
 * Destructor call order: ~Pacman() -> ~PacCharacter()
 * (See slide 23 of your inheritance notes)
 */
Pacman::~Pacman() {
    // Cleanup Pacman-specific resources (if any)
}

/**
 * POLYMORPHISM: Override of base class decideMove()
 * 
 * Pacman is controlled by user input, so this returns STAY.
 * The actual movement direction comes from keyboard input handled elsewhere.
 * 
 * This is DIFFERENT behavior than Ghost::decideMove() - that's polymorphism!
 */
Direction Pacman::decideMove([[maybe_unused]] int pacmanRow, [[maybe_unused]] int pacmanCol) {
    // Pacman doesn't decide its own move - user controls it
    return Direction::STAY;
}


/**
 * Ghost constructor with custom appearance
 * 
 * Uses initialization list to call BASE CLASS CONSTRUCTOR
 * Different ghosts can have different appearances (A, B, C, D, G)
 */
Ghost::Ghost(int row, int col, char appearance) 
    : PacCharacter(row, col, appearance)  // Call base class constructor
{
    // Ghost-specific initialization (if any) goes here
}

/**
 * Ghost constructor with default appearance 'G'
 * 
 * OVERLOADED CONSTRUCTOR - allows creating Ghost with just position
 * Uses initialization list to call BASE CLASS CONSTRUCTOR
 */
Ghost::Ghost(int row, int col) 
    : PacCharacter(row, col, 'G')  // Call base class constructor with default 'G'
{
    // Ghost-specific initialization (if any) goes here
}

/**
 * Ghost destructor
 */
Ghost::~Ghost() {
    // Cleanup Ghost-specific resources (if any)
}

/**
 * POLYMORPHISM: Override of base class decideMove()
 * 
 * Ghost AI: Calculate the best direction to move toward Pacman
 * 
 */
Direction Ghost::decideMove(int pacmanRow, int pacmanCol) {
    // Calculate distance to Pacman
    int dx = pacmanCol - col_;  // Horizontal distance
    int dy = pacmanRow - row_;  // Vertical distance
    
    // Ghost AI logic: Move toward Pacman
    // If on same row, move horizontally
    if (dy == 0) {
        if (dx > 0) return Direction::RIGHT;
        else if (dx < 0) return Direction::LEFT;
        else return Direction::STAY;
    }
    
    // If on same column, move vertically
    if (dx == 0) {
        if (dy > 0) return Direction::DOWN;
        else if (dy < 0) return Direction::UP;
        else return Direction::STAY;
    }
    
    // Not aligned - prioritize the axis with greater distance
    if (abs(dy) > abs(dx)) {
        // Move vertically first
        if (dy > 0) return Direction::DOWN;
        else return Direction::UP;
    } else {
        // Move horizontally first
        if (dx > 0) return Direction::RIGHT;
        else return Direction::LEFT;
    }
}