// ipac_character.h
// Demonstrates INHERITANCE and POLYMORPHISM

#ifndef IPAC_CHARACTER_H
#define IPAC_CHARACTER_H

#include "Direction.h"

// Forward declaration
class Board;

/**
 * BASE CLASS: PacCharacter
 * This is the parent class that defines common behavior for all characters.
 * Both Pacman and Ghost will inherit from this class.
 * 
 * Uses:
 * - Protected members (accessible to derived classes)
 * - Pure virtual function (makes this class abstract)
 * - Virtual destructor (required for polymorphism)
 */
class PacCharacter {
public:
    // Constructor - initializes position and appearance
    PacCharacter(int row, int col, char appearance);

    // Virtual destructor - REQUIRED for proper cleanup with polymorphism
    // When deleting through base pointer, ensures derived destructor is called
    virtual ~PacCharacter();

    // Getter functions - inherited by derived classes
    int getRow() const;
    int getCol() const;
    char getAppearance() const;

    // Setter functions - inherited by derived classes  
    void setPosition(int row, int col);
    void setAppearance(char appearance);

    /**
     * PURE VIRTUAL FUNCTION - Makes PacCharacter an ABSTRACT class
     * 
     * This function MUST be overridden by derived classes.
     * Each derived class (Pacman, Ghost) will have different behavior:
     * - Pacman: Returns STAY (controlled by user input)
     * - Ghost: Returns direction toward Pacman (AI logic)
     * 
     * This demonstrates POLYMORPHISM - same function name, different behavior
     */
    virtual Direction decideMove(int pacmanRow, int pacmanCol) = 0;

protected:
    // PROTECTED members - accessible to derived classes but not to outside code
    // (See slide 19 of your inheritance notes)
    int row_;
    int col_;
    char appearance_;
};


/**
 * DERIVED CLASS: Pacman
 * Pacman "is-a" PacCharacter (inheritance relationship)
 * 
 * Inherits: row_, col_, appearance_, getRow(), getCol(), etc.
 * Overrides: decideMove() - returns STAY since user controls Pacman
 */
class Pacman : public PacCharacter {
public:
    // Constructor - calls base class constructor using initialization list
    // (See slide 22 of your inheritance notes)
    Pacman(int row, int col);

    // Destructor
    ~Pacman();

    /**
     * OVERRIDE of base class pure virtual function
     * Pacman's movement is controlled by user input, not AI
     * So this returns STAY - the actual direction comes from user
     */
    Direction decideMove(int pacmanRow, int pacmanCol) override;
};


/**
 * DERIVED CLASS: Ghost
 * Ghost "is-a" PacCharacter (inheritance relationship)
 * 
 * Inherits: row_, col_, appearance_, getRow(), getCol(), etc.
 * Overrides: decideMove() - returns direction toward Pacman (AI)
 */
class Ghost : public PacCharacter {
public:
    // Constructor with custom appearance
    Ghost(int row, int col, char appearance);
    
    // Overloaded constructor with default appearance 'G'
    // (Allows creating Ghost with just row and col)
    Ghost(int row, int col);

    // Destructor
    ~Ghost();

    /**
     * OVERRIDE of base class pure virtual function
     * Ghost AI: Calculate direction to move toward Pacman
     * 
     * This is different from Pacman's decideMove() - POLYMORPHISM!
     */
    Direction decideMove(int pacmanRow, int pacmanCol) override;
};

#endif // IPAC_CHARACTER_H