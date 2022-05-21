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

    // Given a move squence like [A3-B4],[...],... returns all the moves described
    std::vector<Move>notation_to_moves(const std::string& notation);

    // The same function is found in UI, but it cannot be included here
    Coords convert_coords(const std::string& toConvert);

    // Used by notation_to_moves, converts a string to a move
    Move string_to_move(const std::string& notation);
};