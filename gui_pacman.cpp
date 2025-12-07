// gui_pacman.cpp

#include "gui_pacman.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

/**
 * Constructor - new game
 */
GuiPacman::GuiPacman(int boardSize, const std::string& outputFile)
    : board(nullptr), 
      outputFileName(outputFile),
      boardSize(boardSize),
      lastDirection(Direction::RIGHT),
      gameState(GameState::START_SCREEN),
      lives(INITIAL_LIVES),
      highScore(0),
      totalDots(0),
      dotsEaten(0),
      hasCherryOnBoard(false),
      cherryEaten(false),
      cherryRow(-1),
      cherryCol(-1) {
    
    // Calculate window size
    int windowWidth = boardSize * TILE_SIZE + 2 * PADDING;
    int windowHeight = (boardSize + 2) * TILE_SIZE + 2 * PADDING + 50; // Extra for title and lives
    
    // Create window
    window.create(sf::VideoMode(windowWidth, windowHeight), "Pac-Man");
    window.setFramerateLimit(60);
    
    // Load resources
    loadTextures();
    loadHighScore();
    
    // Load font - try multiple paths for cross-platform support
    if (!font.loadFromFile("/System/Library/Fonts/Helvetica.ttc")) {
        if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf")) {
            if (!font.loadFromFile("/usr/share/fonts/TTF/DejaVuSans-Bold.ttf")) {
                if (!font.loadFromFile("arial.ttf")) {
                    if (!font.loadFromFile("/Library/Fonts/Arial.ttf")) {
                        std::cerr << "Warning: Could not load font" << std::endl;
                    }
                }
            }
        }
    }
}

/**
 * Constructor - load from file
 */
GuiPacman::GuiPacman(const std::string& inputFile, const std::string& outputFile)
    : board(new Board(inputFile)),
      outputFileName(outputFile),
      boardSize(0),
      lastDirection(Direction::RIGHT),
      gameState(GameState::START_SCREEN),
      lives(INITIAL_LIVES),
      highScore(0),
      totalDots(0),
      dotsEaten(0),
      hasCherryOnBoard(false),
      cherryEaten(false),
      cherryRow(-1),
      cherryCol(-1) {
    
    boardSize = board->getGridSize();
    
    // Calculate window size based on loaded board
    int windowWidth = boardSize * TILE_SIZE + 2 * PADDING;
    int windowHeight = (boardSize + 2) * TILE_SIZE + 2 * PADDING + 50;
    
    // Create window
    window.create(sf::VideoMode(windowWidth, windowHeight), "Pac-Man");
    window.setFramerateLimit(60);
    
    // Load resources
    loadTextures();
    loadHighScore();
    
    // Load font
    if (!font.loadFromFile("/System/Library/Fonts/Helvetica.ttc")) {
        if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf")) {
            if (!font.loadFromFile("/usr/share/fonts/TTF/DejaVuSans-Bold.ttf")) {
                if (!font.loadFromFile("arial.ttf")) {
                    if (!font.loadFromFile("/Library/Fonts/Arial.ttf")) {
                        std::cerr << "Warning: Could not load font" << std::endl;
                    }
                }
            }
        }
    }
    
    // Count total dots
    for (int r = 0; r < boardSize; r++) {
        for (int c = 0; c < boardSize; c++) {
            if (board->getGridChar(r, c) == '*') {
                totalDots++;
            }
        }
    }
}

/**
 * Destructor
 */
GuiPacman::~GuiPacman() {
    saveHighScore();
    if (board != nullptr) {
        delete board;
    }
}

/**
 * Initialize/Reset the game
 */
void GuiPacman::initGame() {
    if (board != nullptr) {
        delete board;
    }
    board = new Board(boardSize);
    dotsEaten = 0;
    lastDirection = Direction::RIGHT;
    
    // Reset cherry state BEFORE placing new cherry
    cherryEaten = false;
    hasCherryOnBoard = false;
    cherryRow = -1;
    cherryCol = -1;
    
    // Count total dots
    totalDots = 0;
    for (int r = 0; r < boardSize; r++) {
        for (int c = 0; c < boardSize; c++) {
            if (board->getGridChar(r, c) == '*') {
                totalDots++;
            }
        }
    }
    
    // Place cherry randomly on the board
    srand(static_cast<unsigned>(time(nullptr)));
    placeCherry();
}

/**
 * Load all textures from image files
 */
void GuiPacman::loadTextures() {
    std::map<std::string, std::string> textureFiles = {
        {"pacman_right", "image/pacman_right.png"},
        {"pacman_dead", "image/pacman_dead.png"},
        {"blinky", "image/blinky_left.png"},
        {"clyde", "image/clyde_up.png"},
        {"inky", "image/inky_down.png"},
        {"pinky", "image/pinky_left.png"},
        {"ghostblue", "image/ghostblue.png"},
        {"dot_uneaten", "image/dot_uneaten.png"},
        {"dot_eaten", "image/dot_eaten.png"},
        {"cherry", "image/cherry.png"},
        {"fight_on", "image/fight_on.png"},
        {"beat_bruins", "image/beat_bruins.png"}
    };
    
    for (const auto& pair : textureFiles) {
        sf::Texture texture;
        if (texture.loadFromFile(pair.second)) {
            textures[pair.first] = texture;
        } else {
            std::cerr << "Warning: Could not load texture: " << pair.second << std::endl;
        }
    }
}

/**
 * Main game loop
 */
void GuiPacman::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                if (board != nullptr) {
                    try {
                        board->saveBoard(outputFileName);
                        std::cout << "Game saved to: " << outputFileName << std::endl;
                    } catch (...) {
                        std::cerr << "Error saving board" << std::endl;
                    }
                }
                window.close();
            }
            
            if (event.type == sf::Event::KeyPressed) {
                handleInput(event);
            }
        }
        
        render();
    }
}

/**
 * Handle keyboard input based on game state
 */
void GuiPacman::handleInput(const sf::Event& event) {
    switch (gameState) {
        case GameState::START_SCREEN:
            if (event.key.code == sf::Keyboard::Enter || 
                event.key.code == sf::Keyboard::Space) {
                initGame();
                gameState = GameState::PLAYING;
            }
            if (event.key.code == sf::Keyboard::Escape || 
                event.key.code == sf::Keyboard::Q) {
                window.close();
            }
            break;
            
        case GameState::PLAYING:
            switch (event.key.code) {
                case sf::Keyboard::Up:
                case sf::Keyboard::W:
                    board->move(Direction::UP);
                    lastDirection = Direction::UP;
                    break;
                    
                case sf::Keyboard::Down:
                case sf::Keyboard::S:
                    board->move(Direction::DOWN);
                    lastDirection = Direction::DOWN;
                    break;
                    
                case sf::Keyboard::Left:
                case sf::Keyboard::A:
                    board->move(Direction::LEFT);
                    lastDirection = Direction::LEFT;
                    break;
                    
                case sf::Keyboard::Right:
                case sf::Keyboard::D:
                    board->move(Direction::RIGHT);
                    lastDirection = Direction::RIGHT;
                    break;
                    
                case sf::Keyboard::P:
                case sf::Keyboard::Escape:
                    gameState = GameState::PAUSED;
                    break;
                    
                default:
                    break;
            }
            
            // Check game over
            if (board->getIsGameOver()) {
                lives--;
                if (lives <= 0) {
                    gameState = GameState::GAME_OVER;
                    // Update high score
                    if (board->getScore() > highScore) {
                        highScore = board->getScore();
                        saveHighScore();
                    }
                } else {
                    // Reset positions but keep score
                    initGame();
                }
            }
            
            // Check if cherry was eaten (instant win!)
            if (checkCherryEaten()) {
                // Bonus points for eating cherry
                cherryEaten = true;
                gameState = GameState::YOU_WIN;
                if (board->getScore() > highScore) {
                    highScore = board->getScore();
                    saveHighScore();
                }
            }
            
            // Check win condition (all dots eaten)
            if (checkWinCondition()) {
                gameState = GameState::YOU_WIN;
                if (board->getScore() > highScore) {
                    highScore = board->getScore();
                    saveHighScore();
                }
            }
            break;
            
        case GameState::PAUSED:
            if (event.key.code == sf::Keyboard::P || 
                event.key.code == sf::Keyboard::Escape ||
                event.key.code == sf::Keyboard::Enter) {
                gameState = GameState::PLAYING;
            }
            if (event.key.code == sf::Keyboard::Q) {
                if (board != nullptr) {
                    board->saveBoard(outputFileName);
                }
                window.close();
            }
            break;
            
        case GameState::GAME_OVER:
        case GameState::YOU_WIN:
            if (event.key.code == sf::Keyboard::Enter || 
                event.key.code == sf::Keyboard::Space) {
                // Restart game
                lives = INITIAL_LIVES;
                initGame();
                gameState = GameState::PLAYING;
            }
            if (event.key.code == sf::Keyboard::Escape || 
                event.key.code == sf::Keyboard::Q) {
                window.close();
            }
            break;
    }
}

/**
 * Render the game based on current state
 */
void GuiPacman::render() {
    // Update view to match current window size (fixes resize issues)
    sf::View view(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
    window.setView(view);
    
    window.clear(sf::Color::Black);
    
    switch (gameState) {
        case GameState::START_SCREEN:
            drawStartScreen();
            break;
            
        case GameState::PLAYING:
            drawUI();
            if (board != nullptr) {
                int gridSize = board->getGridSize();
                for (int row = 0; row < gridSize; row++) {
                    for (int col = 0; col < gridSize; col++) {
                        char tileChar = board->getGridChar(row, col);
                        drawTile(tileChar, row, col);
                    }
                }
                // Draw cherry on top if it exists
                if (hasCherryOnBoard && cherryRow >= 0 && cherryCol >= 0) {
                    drawCherry(cherryRow, cherryCol);
                }
            }
            drawLives();
            break;
            
        case GameState::PAUSED:
            drawUI();
            if (board != nullptr) {
                int gridSize = board->getGridSize();
                for (int row = 0; row < gridSize; row++) {
                    for (int col = 0; col < gridSize; col++) {
                        char tileChar = board->getGridChar(row, col);
                        drawTile(tileChar, row, col);
                    }
                }
                // Draw cherry on top if it exists
                if (hasCherryOnBoard && cherryRow >= 0 && cherryCol >= 0) {
                    drawCherry(cherryRow, cherryCol);
                }
            }
            drawLives();
            drawPauseMenu();
            break;
            
        case GameState::GAME_OVER:
            drawUI();
            if (board != nullptr) {
                int gridSize = board->getGridSize();
                for (int row = 0; row < gridSize; row++) {
                    for (int col = 0; col < gridSize; col++) {
                        char tileChar = board->getGridChar(row, col);
                        drawTile(tileChar, row, col);
                    }
                }
            }
            drawLives();
            drawGameOver();
            break;
            
        case GameState::YOU_WIN:
            drawUI();
            if (board != nullptr) {
                int gridSize = board->getGridSize();
                for (int row = 0; row < gridSize; row++) {
                    for (int col = 0; col < gridSize; col++) {
                        char tileChar = board->getGridChar(row, col);
                        drawTile(tileChar, row, col);
                    }
                }
            }
            drawLives();
            drawWinScreen();
            break;
    }
    
    window.display();
}

/**
 * Draw start screen
 */
void GuiPacman::drawStartScreen() {
    // Get window dimensions for dynamic scaling
    float winW = static_cast<float>(window.getSize().x);
    float winH = static_cast<float>(window.getSize().y);
    
    // Scale factor based on window size (baseline: 500px width)
    float scaleFactor = winW / 500.0f;
    if (scaleFactor < 0.5f) scaleFactor = 0.5f;  // Minimum scale
    if (scaleFactor > 2.0f) scaleFactor = 2.0f;  // Maximum scale
    
    // "BEAT THE BRUINS" banner at top
    if (textures.count("beat_bruins")) {
        sf::Sprite banner;
        banner.setTexture(textures["beat_bruins"]);
        sf::Vector2u texSize = textures["beat_bruins"].getSize();
        float scale = (winW * 0.7f) / texSize.x;
        if (scale > 1.5f) scale = 1.5f;  // Cap the scale
        banner.setScale(scale, scale);
        banner.setOrigin(texSize.x / 2.0f, texSize.y / 2.0f);
        banner.setPosition(winW / 2.0f, winH * 0.08f);
        window.draw(banner);
    } else {
        sf::Text banner;
        banner.setFont(font);
        banner.setString("BEAT THE BRUINS!");
        banner.setCharacterSize(static_cast<unsigned int>(30 * scaleFactor));
        banner.setFillColor(sf::Color(153, 27, 30));
        banner.setStyle(sf::Text::Bold);
        sf::FloatRect bannerBounds = banner.getLocalBounds();
        banner.setOrigin(bannerBounds.width / 2, bannerBounds.height / 2);
        banner.setPosition(winW / 2.0f, winH * 0.08f);
        window.draw(banner);
    }
    
    // Title - TROJAN PAC-MAN (positioned ABOVE the Pac-Man image)
    sf::Text title;
    title.setFont(font);
    title.setString("USC PAC-MAN");
    title.setCharacterSize(static_cast<unsigned int>(35 * scaleFactor));
    title.setFillColor(sf::Color(255, 204, 0));
    title.setStyle(sf::Text::Bold);
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setOrigin(titleBounds.width / 2, titleBounds.height / 2);
    title.setPosition(winW / 2.0f, winH * 0.18f);
    window.draw(title);
    
    // Draw Trojan Pac-Man image - dynamically sized
    if (textures.count("pacman_right")) {
        sf::Sprite pacman;
        pacman.setTexture(textures["pacman_right"]);
        sf::Vector2u texSize = textures["pacman_right"].getSize();
        pacman.setOrigin(texSize.x / 2.0f, texSize.y / 2.0f);
        pacman.setPosition(winW / 2.0f, winH * 0.45f);
        
        // Scale Pac-Man based on window size
        float pacScale = (winW * 0.4f) / texSize.x;
        if (pacScale > 1.2f) pacScale = 1.2f;  // Cap maximum size
        if (pacScale < 0.3f) pacScale = 0.3f;  // Minimum size
        pacman.setScale(pacScale, pacScale);
        window.draw(pacman);
    }
    
    // High Score - YELLOW color for visibility
    sf::Text highScoreText;
    highScoreText.setFont(font);
    highScoreText.setString("High Score: " + std::to_string(highScore));
    highScoreText.setCharacterSize(static_cast<unsigned int>(22 * scaleFactor));
    highScoreText.setFillColor(sf::Color(255, 204, 0));
    highScoreText.setStyle(sf::Text::Bold);
    sf::FloatRect hsBounds = highScoreText.getLocalBounds();
    highScoreText.setOrigin(hsBounds.width / 2, hsBounds.height / 2);
    highScoreText.setPosition(winW / 2.0f, winH * 0.72f);
    window.draw(highScoreText);
    
    // Instructions
    sf::Text startText;
    startText.setFont(font);
    startText.setString("Press ENTER or SPACE to Start");
    startText.setCharacterSize(static_cast<unsigned int>(18 * scaleFactor));
    startText.setFillColor(sf::Color(255, 204, 0));
    sf::FloatRect startBounds = startText.getLocalBounds();
    startText.setOrigin(startBounds.width / 2, startBounds.height / 2);
    startText.setPosition(winW / 2.0f, winH * 0.82f);
    window.draw(startText);
    
    // Controls info
    sf::Text controlsText;
    controlsText.setFont(font);
    controlsText.setString("Controls: Arrow Keys or WASD | P to Pause | ESC to Quit");
    controlsText.setCharacterSize(static_cast<unsigned int>(12 * scaleFactor));
    controlsText.setFillColor(sf::Color(180, 180, 180));
    sf::FloatRect ctrlBounds = controlsText.getLocalBounds();
    controlsText.setOrigin(ctrlBounds.width / 2, ctrlBounds.height / 2);
    controlsText.setPosition(winW / 2.0f, winH * 0.90f);
    window.draw(controlsText);
    
    // Co-author credits
    sf::Text creditsText;
    creditsText.setFont(font);
    creditsText.setString("Co-authored by Tony Yoo & Pranet Jagtap");
    creditsText.setCharacterSize(static_cast<unsigned int>(10 * scaleFactor));
    creditsText.setFillColor(sf::Color(140, 140, 140));
    sf::FloatRect creditsBounds = creditsText.getLocalBounds();
    creditsText.setOrigin(creditsBounds.width / 2, creditsBounds.height / 2);
    creditsText.setPosition(winW / 2.0f, winH * 0.96f);
    window.draw(creditsText);
}

/**
 * Draw pause menu
 */
void GuiPacman::drawPauseMenu() {
    // Semi-transparent overlay
    sf::RectangleShape overlay(sf::Vector2f(window.getSize().x, window.getSize().y));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(overlay);
    
    // Pause text
    sf::Text pauseText;
    pauseText.setFont(font);
    pauseText.setString("PAUSED");
    pauseText.setCharacterSize(50);
    pauseText.setFillColor(sf::Color::Yellow);
    pauseText.setStyle(sf::Text::Bold);
    sf::FloatRect pauseBounds = pauseText.getLocalBounds();
    pauseText.setOrigin(pauseBounds.width / 2, pauseBounds.height / 2);
    pauseText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f - 30);
    window.draw(pauseText);
    
    // Resume instruction
    sf::Text resumeText;
    resumeText.setFont(font);
    resumeText.setString("Press P or ENTER to Resume");
    resumeText.setCharacterSize(20);
    resumeText.setFillColor(sf::Color::White);
    sf::FloatRect resumeBounds = resumeText.getLocalBounds();
    resumeText.setOrigin(resumeBounds.width / 2, resumeBounds.height / 2);
    resumeText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f + 30);
    window.draw(resumeText);
    
    // Quit instruction
    sf::Text quitText;
    quitText.setFont(font);
    quitText.setString("Press Q to Save and Quit");
    quitText.setCharacterSize(18);
    quitText.setFillColor(sf::Color(200, 200, 200));
    sf::FloatRect quitBounds = quitText.getLocalBounds();
    quitText.setOrigin(quitBounds.width / 2, quitBounds.height / 2);
    quitText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f + 70);
    window.draw(quitText);
}

/**
 * Draw game over overlay - Shows "BEAT THE BRUINS" as motivation
 */
void GuiPacman::drawGameOver() {
    // Get window dimensions for dynamic scaling
    float winW = static_cast<float>(window.getSize().x);
    float winH = static_cast<float>(window.getSize().y);
    
    // Scale factor based on window size
    float scaleFactor = winW / 500.0f;
    if (scaleFactor < 0.5f) scaleFactor = 0.5f;
    if (scaleFactor > 2.0f) scaleFactor = 2.0f;
    
    // Full screen UCLA blue overlay
    sf::RectangleShape overlay(sf::Vector2f(winW, winH));
    overlay.setFillColor(sf::Color(39, 116, 174, 220)); // UCLA Blue
    window.draw(overlay);
    
    // "BEAT THE BRUINS" image or text - dynamically scaled
    if (textures.count("beat_bruins")) {
        sf::Sprite beatBruins;
        beatBruins.setTexture(textures["beat_bruins"]);
        sf::Vector2u texSize = textures["beat_bruins"].getSize();
        float scale = (winW * 0.6f) / texSize.x;
        if (scale > 1.5f) scale = 1.5f;
        beatBruins.setScale(scale, scale);
        beatBruins.setOrigin(texSize.x / 2.0f, texSize.y / 2.0f);
        beatBruins.setPosition(winW / 2.0f, winH * 0.25f);
        window.draw(beatBruins);
    } else {
        sf::Text beatText;
        beatText.setFont(font);
        beatText.setString("BEAT THE BRUINS!");
        beatText.setCharacterSize(static_cast<unsigned int>(35 * scaleFactor));
        beatText.setFillColor(sf::Color::White);
        beatText.setStyle(sf::Text::Bold);
        sf::FloatRect beatBounds = beatText.getLocalBounds();
        beatText.setOrigin(beatBounds.width / 2, beatBounds.height / 2);
        beatText.setPosition(winW / 2.0f, winH * 0.25f);
        window.draw(beatText);
    }
    
    // "THE BRUINS GOT YOU!" text
    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setString("THE BRUINS GOT YOU!");
    gameOverText.setCharacterSize(static_cast<unsigned int>(24 * scaleFactor));
    gameOverText.setFillColor(sf::Color(255, 204, 0));
    gameOverText.setStyle(sf::Text::Bold);
    sf::FloatRect goBounds = gameOverText.getLocalBounds();
    gameOverText.setOrigin(goBounds.width / 2, goBounds.height / 2);
    gameOverText.setPosition(winW / 2.0f, winH * 0.45f);
    window.draw(gameOverText);
    
    // Final score
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setString("Final Score: " + std::to_string(board->getScore()));
    scoreText.setCharacterSize(static_cast<unsigned int>(20 * scaleFactor));
    scoreText.setFillColor(sf::Color::White);
    sf::FloatRect scoreBounds = scoreText.getLocalBounds();
    scoreText.setOrigin(scoreBounds.width / 2, scoreBounds.height / 2);
    scoreText.setPosition(winW / 2.0f, winH * 0.55f);
    window.draw(scoreText);
    
    // High score
    sf::Text highScoreText;
    highScoreText.setFont(font);
    std::string hsString = "High Score: " + std::to_string(highScore);
    if (board->getScore() >= highScore) {
        hsString = "NEW HIGH SCORE!";
        highScoreText.setFillColor(sf::Color::Yellow);
    } else {
        highScoreText.setFillColor(sf::Color(200, 200, 200));
    }
    highScoreText.setString(hsString);
    highScoreText.setCharacterSize(static_cast<unsigned int>(18 * scaleFactor));
    sf::FloatRect hsBounds = highScoreText.getLocalBounds();
    highScoreText.setOrigin(hsBounds.width / 2, hsBounds.height / 2);
    highScoreText.setPosition(winW / 2.0f, winH * 0.65f);
    window.draw(highScoreText);
    
    // Restart instruction
    sf::Text restartText;
    restartText.setFont(font);
    restartText.setString("Press ENTER to Try Again | ESC to Quit");
    restartText.setCharacterSize(static_cast<unsigned int>(14 * scaleFactor));
    restartText.setFillColor(sf::Color::White);
    sf::FloatRect restartBounds = restartText.getLocalBounds();
    restartText.setOrigin(restartBounds.width / 2, restartBounds.height / 2);
    restartText.setPosition(winW / 2.0f, winH * 0.78f);
    window.draw(restartText);
}

/**
 * Draw win screen
 */
void GuiPacman::drawWinScreen() {
    // Get window dimensions for dynamic scaling
    float winW = static_cast<float>(window.getSize().x);
    float winH = static_cast<float>(window.getSize().y);
    
    // Scale factor based on window size
    float scaleFactor = winW / 500.0f;
    if (scaleFactor < 0.5f) scaleFactor = 0.5f;
    if (scaleFactor > 2.0f) scaleFactor = 2.0f;
    
    // Full screen USC Cardinal overlay
    sf::RectangleShape overlay(sf::Vector2f(winW, winH));
    overlay.setFillColor(sf::Color(153, 27, 30, 220)); // USC Cardinal
    window.draw(overlay);
    
    // "FIGHT ON!" image or text - dynamically scaled
    if (textures.count("fight_on")) {
        sf::Sprite fightOn;
        fightOn.setTexture(textures["fight_on"]);
        sf::Vector2u texSize = textures["fight_on"].getSize();
        float scale = (winW * 0.6f) / texSize.x;
        if (scale > 1.5f) scale = 1.5f;
        fightOn.setScale(scale, scale);
        fightOn.setOrigin(texSize.x / 2.0f, texSize.y / 2.0f);
        fightOn.setPosition(winW / 2.0f, winH * 0.25f);
        window.draw(fightOn);
    } else {
        sf::Text winText;
        winText.setFont(font);
        winText.setString("FIGHT ON!");
        winText.setCharacterSize(static_cast<unsigned int>(50 * scaleFactor));
        winText.setFillColor(sf::Color(255, 204, 0));
        winText.setStyle(sf::Text::Bold);
        sf::FloatRect winBounds = winText.getLocalBounds();
        winText.setOrigin(winBounds.width / 2, winBounds.height / 2);
        winText.setPosition(winW / 2.0f, winH * 0.25f);
        window.draw(winText);
    }
    
    // "YOU BEAT THE BRUINS!" text
    sf::Text beatText;
    beatText.setFont(font);
    beatText.setString("YOU BEAT THE BRUINS!");
    beatText.setCharacterSize(static_cast<unsigned int>(24 * scaleFactor));
    beatText.setFillColor(sf::Color(255, 204, 0));
    beatText.setStyle(sf::Text::Bold);
    sf::FloatRect beatBounds = beatText.getLocalBounds();
    beatText.setOrigin(beatBounds.width / 2, beatBounds.height / 2);
    beatText.setPosition(winW / 2.0f, winH * 0.45f);
    window.draw(beatText);
    
    // Final score
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setString("Final Score: " + std::to_string(board->getScore()));
    scoreText.setCharacterSize(static_cast<unsigned int>(20 * scaleFactor));
    scoreText.setFillColor(sf::Color::White);
    sf::FloatRect scoreBounds = scoreText.getLocalBounds();
    scoreText.setOrigin(scoreBounds.width / 2, scoreBounds.height / 2);
    scoreText.setPosition(winW / 2.0f, winH * 0.55f);
    window.draw(scoreText);
    
    // High score
    sf::Text highScoreText;
    highScoreText.setFont(font);
    std::string hsString = "High Score: " + std::to_string(highScore);
    if (board->getScore() >= highScore) {
        hsString = "NEW HIGH SCORE!";
        highScoreText.setFillColor(sf::Color::Yellow);
    } else {
        highScoreText.setFillColor(sf::Color(200, 200, 200));
    }
    highScoreText.setString(hsString);
    highScoreText.setCharacterSize(static_cast<unsigned int>(18 * scaleFactor));
    sf::FloatRect hsBounds = highScoreText.getLocalBounds();
    highScoreText.setOrigin(hsBounds.width / 2, hsBounds.height / 2);
    highScoreText.setPosition(winW / 2.0f, winH * 0.65f);
    window.draw(highScoreText);
    
    // Restart instruction
    sf::Text restartText;
    restartText.setFont(font);
    restartText.setString("Press ENTER to Play Again | ESC to Quit");
    restartText.setCharacterSize(static_cast<unsigned int>(14 * scaleFactor));
    restartText.setFillColor(sf::Color::White);
    sf::FloatRect restartBounds = restartText.getLocalBounds();
    restartText.setOrigin(restartBounds.width / 2, restartBounds.height / 2);
    restartText.setPosition(winW / 2.0f, winH * 0.78f);
    window.draw(restartText);
}

/**
 * Draw a single tile
 */
void GuiPacman::drawTile(char tileChar, int row, int col) {
    sf::Sprite sprite;
    bool hasTexture = true;
    float rotation = 0;
    
    switch (tileChar) {
        case 'P':
            if (textures.count("pacman_right")) {
                sprite.setTexture(textures["pacman_right"]);
                rotation = getPacmanRotation();
            } else {
                hasTexture = false;
            }
            break;
            
        case 'X':
            if (textures.count("pacman_dead")) {
                sprite.setTexture(textures["pacman_dead"]);
            } else {
                hasTexture = false;
            }
            break;
            
        case 'G':
            if (textures.count("blinky")) {
                sprite.setTexture(textures["blinky"]);
            } else {
                hasTexture = false;
            }
            break;
            
        case 'A':
            if (textures.count("blinky")) {
                sprite.setTexture(textures["blinky"]);
            } else {
                hasTexture = false;
            }
            break;
            
        case 'B':
            if (textures.count("clyde")) {
                sprite.setTexture(textures["clyde"]);
            } else {
                hasTexture = false;
            }
            break;
            
        case 'C':
            if (textures.count("inky")) {
                sprite.setTexture(textures["inky"]);
            } else {
                hasTexture = false;
            }
            break;
            
        case 'D':
            if (textures.count("pinky")) {
                sprite.setTexture(textures["pinky"]);
            } else {
                hasTexture = false;
            }
            break;
            
        case '*':
            if (textures.count("dot_uneaten")) {
                sprite.setTexture(textures["dot_uneaten"]);
            } else {
                hasTexture = false;
            }
            break;
            
        case ' ':
            if (textures.count("dot_eaten")) {
                sprite.setTexture(textures["dot_eaten"]);
            } else {
                hasTexture = false;
            }
            break;
            
        case 'F':
            if (textures.count("cherry")) {
                sprite.setTexture(textures["cherry"]);
            } else {
                hasTexture = false;
            }
            break;
            
        default:
            hasTexture = false;
            break;
    }
    
    if (hasTexture) {
        sf::Vector2u textureSize = sprite.getTexture()->getSize();
        float scaleX = static_cast<float>(TILE_SIZE) / textureSize.x;
        float scaleY = static_cast<float>(TILE_SIZE) / textureSize.y;
        sprite.setScale(scaleX, scaleY);
        
        float x = PADDING + col * TILE_SIZE;
        float y = PADDING + 50 + row * TILE_SIZE;
        
        if (rotation != 0) {
            sprite.setOrigin(textureSize.x / 2.0f, textureSize.y / 2.0f);
            sprite.setPosition(x + TILE_SIZE / 2.0f, y + TILE_SIZE / 2.0f);
            sprite.setRotation(rotation);
        } else {
            sprite.setPosition(x, y);
        }
        
        window.draw(sprite);
    } else {
        sf::RectangleShape rect(sf::Vector2f(TILE_SIZE - 2, TILE_SIZE - 2));
        float x = PADDING + col * TILE_SIZE + 1;
        float y = PADDING + 50 + row * TILE_SIZE + 1;
        rect.setPosition(x, y);
        
        switch (tileChar) {
            case 'P':
                rect.setFillColor(sf::Color::Yellow);
                break;
            case 'G':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
                rect.setFillColor(sf::Color::Red);
                break;
            case 'X':
                rect.setFillColor(sf::Color(255, 165, 0));
                break;
            case '*':
                rect.setFillColor(sf::Color::White);
                break;
            default:
                rect.setFillColor(sf::Color::Black);
                break;
        }
        
        window.draw(rect);
    }
}

/**
 * Draw title, score, and high score
 */
void GuiPacman::drawUI() {
    // Title - USC themed
    sf::Text title;
    title.setFont(font);
    title.setString("TROJAN PAC-MAN");
    title.setCharacterSize(24);
    title.setFillColor(sf::Color(255, 204, 0)); // USC Gold
    title.setPosition(PADDING, 10);
    window.draw(title);
    
    // Current Score
    sf::Text score;
    score.setFont(font);
    score.setString("Score: " + std::to_string(board != nullptr ? board->getScore() : 0));
    score.setCharacterSize(18);
    score.setFillColor(sf::Color::White);
    float scoreX = window.getSize().x / 2.0f - 40;
    score.setPosition(scoreX, 15);
    window.draw(score);
    
    // High Score
    sf::Text highScoreText;
    highScoreText.setFont(font);
    highScoreText.setString("HI: " + std::to_string(highScore));
    highScoreText.setCharacterSize(18);
    highScoreText.setFillColor(sf::Color(153, 27, 30)); // USC Cardinal
    float hsX = window.getSize().x - 100;
    highScoreText.setPosition(hsX, 15);
    window.draw(highScoreText);
}

/**
 * Draw lives as Pac-Man icons
 */
void GuiPacman::drawLives() {
    float y = window.getSize().y - TILE_SIZE - 5;
    
    // Lives label
    sf::Text livesLabel;
    livesLabel.setFont(font);
    livesLabel.setString("Lives:");
    livesLabel.setCharacterSize(16);
    livesLabel.setFillColor(sf::Color::White);
    livesLabel.setPosition(PADDING, y + 15);
    window.draw(livesLabel);
    
    // Draw Pac-Man icons for each life
    for (int i = 0; i < lives; i++) {
        if (textures.count("pacman_right")) {
            sf::Sprite lifeIcon;
            lifeIcon.setTexture(textures["pacman_right"]);
            sf::Vector2u texSize = textures["pacman_right"].getSize();
            float scale = 30.0f / texSize.x;
            lifeIcon.setScale(scale, scale);
            lifeIcon.setPosition(PADDING + 60 + i * 35, y + 10);
            window.draw(lifeIcon);
        } else {
            sf::CircleShape lifeCircle(12);
            lifeCircle.setFillColor(sf::Color::Yellow);
            lifeCircle.setPosition(PADDING + 60 + i * 35, y + 12);
            window.draw(lifeCircle);
        }
    }
}

/**
 * Check if all dots have been eaten
 */
bool GuiPacman::checkWinCondition() {
    if (board == nullptr) return false;
    
    int gridSize = board->getGridSize();
    for (int r = 0; r < gridSize; r++) {
        for (int c = 0; c < gridSize; c++) {
            if (board->getGridChar(r, c) == '*') {
                return false; // Still dots remaining
            }
        }
    }
    return true; // All dots eaten
}

/**
 * Check if cherry was eaten (Pac-Man moved to cherry position)
 */
bool GuiPacman::checkCherryEaten() {
    // Don't check if cherry was never placed or already eaten
    if (!hasCherryOnBoard) return false;
    if (board == nullptr) return false;
    if (cherryRow < 0 || cherryCol < 0) return false;
    
    // Get Pac-Man's current position by finding 'P' on the grid
    int pacRow = -1, pacCol = -1;
    int gridSize = board->getGridSize();
    
    for (int r = 0; r < gridSize && pacRow < 0; r++) {
        for (int c = 0; c < gridSize && pacCol < 0; c++) {
            char ch = board->getGridChar(r, c);
            if (ch == 'P') {
                pacRow = r;
                pacCol = c;
            }
        }
    }
    
    // Check if Pac-Man is at the cherry position
    if (pacRow == cherryRow && pacCol == cherryCol) {
        hasCherryOnBoard = false;
        return true;
    }
    
    return false;
}

/**
 * Place cherry randomly on the board (just track position, don't modify grid)
 */
void GuiPacman::placeCherry() {
    if (board == nullptr) return;
    
    int gridSize = board->getGridSize();
    int attempts = 0;
    
    // Reset cherry state
    hasCherryOnBoard = false;
    cherryRow = -1;
    cherryCol = -1;
    
    while (attempts < 100) {
        int r = rand() % gridSize;
        int c = rand() % gridSize;
        
        // Place cherry on a dot position (not on Pac-Man or ghosts)
        char currentChar = board->getGridChar(r, c);
        if (currentChar == '*') {
            // Avoid placing too close to center (where Pac-Man starts)
            int center = gridSize / 2;
            if (abs(r - center) > 2 || abs(c - center) > 2) {
                // Don't modify the grid - just track the position
                // The cherry will be drawn on top during render
                hasCherryOnBoard = true;
                cherryRow = r;
                cherryCol = c;
                return;
            }
        }
        attempts++;
    }
    
    // Fallback: place cherry somewhere valid
    for (int r = 0; r < gridSize && !hasCherryOnBoard; r++) {
        for (int c = 0; c < gridSize && !hasCherryOnBoard; c++) {
            char ch = board->getGridChar(r, c);
            int center = gridSize / 2;
            if (ch == '*' && (abs(r - center) > 1 || abs(c - center) > 1)) {
                hasCherryOnBoard = true;
                cherryRow = r;
                cherryCol = c;
            }
        }
    }
}

/**
 * Draw cherry at specified position
 */
void GuiPacman::drawCherry(int row, int col) {
    if (!textures.count("cherry")) return;
    
    sf::Sprite sprite;
    sprite.setTexture(textures["cherry"]);
    
    sf::Vector2u textureSize = sprite.getTexture()->getSize();
    float scaleX = static_cast<float>(TILE_SIZE) / textureSize.x;
    float scaleY = static_cast<float>(TILE_SIZE) / textureSize.y;
    sprite.setScale(scaleX, scaleY);
    
    float x = PADDING + col * TILE_SIZE;
    float y = PADDING + 50 + row * TILE_SIZE;
    sprite.setPosition(x, y);
    
    window.draw(sprite);
}

/**
 * Get Pac-Man rotation based on last direction
 */
float GuiPacman::getPacmanRotation() {
    switch (lastDirection) {
        case Direction::UP:
            return 270.0f;
        case Direction::DOWN:
            return 90.0f;
        case Direction::LEFT:
            return 180.0f;
        case Direction::RIGHT:
        default:
            return 0.0f;
    }
}

/**
 * Load high score from file
 */
void GuiPacman::loadHighScore() {
    std::ifstream file("highscore.dat");
    if (file.is_open()) {
        file >> highScore;
        file.close();
    } else {
        highScore = 0;
    }
}

/**
 * Save high score to file
 */
void GuiPacman::saveHighScore() {
    std::ofstream file("highscore.dat");
    if (file.is_open()) {
        file << highScore;
        file.close();
    }
}