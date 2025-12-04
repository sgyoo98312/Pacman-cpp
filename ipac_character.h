// ipac_character.h

#ifndef IPAC_CHARACTER_H
#define IPAC_CHARACTER_H

#include "Direction.h"

/**
 * Base class for all Pac-Man characters (Pacman and Ghosts)
 * This class demonstrates INHERITANCE - derived classes will extend this
 */
class PacCharacter {
public:
    // Constructor
    PacCharacter(int row, int col, char appearance);

    // Virtual destructor - important for polymorphism
    virtual ~PacCharacter() = default;

    // Getter for row (vertical coordinate)
    int getRow() const;

    // Getter for column (horizontal coordinate)
    int getCol() const;

    // Getter for appearance character
    char getAppearance() const;

    // Setter for position
    void setPosition(int row, int col);

    // Setter for appearance
    void setAppearance(char appearance);

    /**
     * Pure virtual function for deciding movement
     * This enables POLYMORPHISM - each derived class implements differently
     * @param targetRow The row to move towards (Pacman's position for ghosts)
     * @param targetCol The column to move towards
     * @return The direction to move
     */
    virtual Direction decideMove(int targetRow, int targetCol) = 0;

protected:
    int row_;
    int col_;
    char appearance_;
};

/**
 * Pacman class - controlled by the user
 * Demonstrates INHERITANCE from PacCharacter
 */
class Pacman : public PacCharacter {
public:
    Pacman(int row, int col);
    
    // POLYMORPHISM: Pacman doesn't auto-move, controlled by user
    Direction decideMove(int targetRow, int targetCol) override;
};

/**
 * Ghost class - AI controlled, moves toward Pacman
 * Demonstrates INHERITANCE from PacCharacter
 */
class Ghost : public PacCharacter {
public:
    Ghost(int row, int col);
    
    // POLYMORPHISM: Ghost moves toward target (Pacman)
    Direction decideMove(int targetRow, int targetCol) override;
};

#endif // IPAC_CHARACTER_H