// gui_pacman.h

#ifndef GUI_PACMAN_H
#define GUI_PACMAN_H

#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include "Board.h"
#include "Direction.h"

// Game states
enum class GameState {
    START_SCREEN,
    PLAYING,
    PAUSED,
    GAME_OVER,
    YOU_WIN
};

/**
 * GuiPacman class - Graphical version of Pac-Man using SFML
 * Converted from Java/JavaFX to C++/SFML
 */
class GuiPacman {
public:
    /**
     * Constructor
     * @param boardSize Size of the game board
     * @param outputFile File to save the game to
     */
    GuiPacman(int boardSize, const std::string& outputFile);

    /**
     * Constructor - load from file
     * @param inputFile File to load the game from
     * @param outputFile File to save the game to
     */
    GuiPacman(const std::string& inputFile, const std::string& outputFile);

    /**
     * Destructor
     */
    ~GuiPacman();

    /**
     * Run the game loop
     */
    void run();

private:
    // Constants
    static const int TILE_SIZE = 50;
    static const int PADDING = 10;
    static const int INITIAL_LIVES = 3;
    
    // Game components
    Board* board;
    std::string outputFileName;
    int boardSize;
    
    // SFML components
    sf::RenderWindow window;
    sf::Font font;
    
    // Textures for game elements
    std::map<std::string, sf::Texture> textures;
    
    // Direction tracking for Pac-Man rotation
    Direction lastDirection;
    
    // Game state
    GameState gameState;
    int lives;
    int highScore;
    int totalDots;
    int dotsEaten;
    
    // Cherry power-up
    bool hasCherryOnBoard;
    bool cherryEaten;
    int cherryRow;
    int cherryCol;

    /**
     * Initialize/Reset the game
     */
    void initGame();

    /**
     * Load all image textures
     */
    void loadTextures();

    /**
     * Handle keyboard input
     * @param event The SFML event
     */
    void handleInput(const sf::Event& event);

    /**
     * Render the game
     */
    void render();

    /**
     * Draw a tile at the specified position
     * @param tileChar The character representation of the tile
     * @param row Row position
     * @param col Column position
     */
    void drawTile(char tileChar, int row, int col);

    /**
     * Draw the score, title, and lives
     */
    void drawUI();

    /**
     * Draw start screen
     */
    void drawStartScreen();

    /**
     * Draw pause menu
     */
    void drawPauseMenu();

    /**
     * Draw game over overlay
     */
    void drawGameOver();

    /**
     * Draw win screen
     */
    void drawWinScreen();

    /**
     * Draw lives (Pac-Man icons)
     */
    void drawLives();

    /**
     * Check if player has won (all dots eaten)
     */
    bool checkWinCondition();

    /**
     * Check if cherry was eaten (instant win!)
     */
    bool checkCherryEaten();

    /**
     * Place cherry randomly on the board
     */
    void placeCherry();

    /**
     * Draw cherry at specified position
     */
    void drawCherry(int row, int col);

    /**
     * Get rotation angle for Pac-Man based on direction
     * @return Rotation angle in degrees
     */
    float getPacmanRotation();

    /**
     * Reset Pac-Man position after losing a life
     */
    void resetPositions();

    /**
     * Load/Save high score
     */
    void loadHighScore();
    void saveHighScore();
};

#endif // GUI_PACMAN_H