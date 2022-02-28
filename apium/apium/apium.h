#pragma once

#include "../scoredmove/scoredMove.h"
#include "../../engine/engine.h"
#include "../a_piece/a_piece.h"
#include "../playstyle/playstyle.h"

class Apium {
    // Used as a weight to calculate the position's score
    Playstyle playstyle;

    // The moves Apium comes up with and actually performs, if the opponent does something expected you don't need to recalculate
    std::vector<Move> moves;

    // A list of all the pieces and their locations, so you don't have to look for the every time by looping the whole board
    std::vector<Square> pieceCoords;

    // Used for simulate_functions and access enemy logs
    GameEngine engine;
public:
    PlayerColor playerColor;

    void evaluate_moves();

    Apium(Board& board, GameEngine& engine);
};