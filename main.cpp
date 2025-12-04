// main.cpp
// Entry point for the Pac-Man game

#include "game_pacman.h"

int main(int argc, char** argv) {
    GamePacman game;
    game.play(argc, argv);
    return 0;
}