#pragma once
#include <fstream>
#include <string>
#include "../enums/fileIssue.h"
#include "../engine/engine.h"

namespace FileHandler {
    /** engine provides player names and move history
     *  gameState describes the state of the game at the moment it is being saved:
     *  0 = Game is still going on
     *  1 = White won
     *  2 = Black won
     *  3 = Draw
     */
     fileIssue create_file(const GameEngine& engine, const std::string& gameName, std::string& filePath, const int gameState);

     void print_move_sequence(const GameEngine& engine);
};