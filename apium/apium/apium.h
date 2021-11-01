#pragma once

#include "../scoredmove/scoredMove.h"
#include "../../engine/engine.h"

/*
 *  PROGRESSION TABLE
 *  BLOW    | Always
 *  MOVE    | +0.5 every row (from 1)
 *  EAT     | 1
 *  DAMONE  | 2
 * */

// Will have to update GameEngine every turn

class Apium {
private:
    // The moves Apium comes up with and actually performs, if the opponent does something expected you don't need to recalculate
    std::vector<Move> moves;
    // A list of all the pieces and their locations, so you don't have to look for the every time by looping the whole board
    std::vector<Square> pieceCoords;
    // Used for simulate_functions and access enemy logs
    GameEngine engine;
public:
    void evaluate_moves();

    Apium(Board& board, GameEngine& engine);
};