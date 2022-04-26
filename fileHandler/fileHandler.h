#pragma once
#include <fstream>
#include <string>
#include "../enums/fileIssue.h"
#include "../engine/engine.h"

class FileHandler {
public:
    /** engine provides player names and move history
     *  gameState describes the state of the game at the moment it is being saved:
     *  0 = Game is still going on
     *  1 = White won
     *  2 = Black won
     *  3 = Draw
     */
    static fileIssue create_file(GameEngine& engine, const std::string& name, std::string& filePath, const int gameState);
};