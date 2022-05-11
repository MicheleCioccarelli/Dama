#pragma once

#include "../scoredmove/scoredMove.h"
#include "../../boardPos/boardPos.h"
#include "../a_piece/a_piece.h"
#include "../playstyle/playstyle.h"

class Apium {
    float m_eval;

    // Used as a weight to calculate the position's score
    Playstyle m_playstyle;

    // The moves Apium comes up with and actually performs, if the opponent does something expected you don't need to recalculate
    std::vector<Move> m_moves;

    // A list of all the pieces and their locations, so you don't have to look for the every time by looping the whole board
    //std::vector<Square> pieceCoords;

    // Used for simulate_functions and access enemy logs
    GameEngine m_engine;
public:
    float get_eval() const;

    void set_eval (float eval);

    /* Returns the vsalue of the piece (positive for white pieces and negative for black ones).
     * The if piece is not provided, the one in m_engine.board.matrix at the coordinates provided is used
     */
    float evaluate_piece(Coords pieceCoords, Piece piece = Piece());

    // Evaluates the board stored in Apium's engine
    float evaluate_current_position();

    // Evaluate a board given in BoardPos notation
    float evaluate_board_position(std::string& currentBoadPos);

    PlayerColor playerColor;

    void evaluate_moves();

    Apium(GameEngine& engine);

    Apium() = default;
};