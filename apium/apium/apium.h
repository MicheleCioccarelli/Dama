#pragma once

#include "../scoredmove/scoredMove.h"
#include "../../engine/engine.h"

class Apium {
private:
    // The moves Apium comes up with and actually performs
    std::vector<Move> moves;
    // A list of all the pieces and their locations, so you don't have to look for the every time by looping the whole board
    std::vector<Square> pieceCoords;
    // Used for simulate_functions
    GameEngine engine;
public:

    Apium(Board& board);
};