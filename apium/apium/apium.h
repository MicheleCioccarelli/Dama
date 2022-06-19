#pragma once

#include "../apiumline/apiumline.h"
#include "../../boardPos/boardPos.h"
#include "../../engine/engine.h"
#include "../a_piece/a_piece.h"
#include "../../enums/playstyle.h"

class Apium {
public:
    Apium() = default;
    Apium(GameEngine& engine);
    Apium(GameEngine& engine, Playstyle playstyle);

    float get_eval() const;
    void set_eval (float eval);

    // Copy the rhs engine into Apium's engine
    void setEngine(const GameEngine& rhs);

    void set_playstyle(Playstyle playstyle);

    /* Returns the value of the piece (positive for white pieces and negative for black ones).
     * The if piece is not provided, the one in m_engine.board.matrix at the coordinates provided is used
     */
    float evaluate_piece(Coords pieceCoords, Piece piece = Piece()) const;

    // Runs an evaluation of the current position and evaluates the internal evaluation
    void update_eval();

    // Evaluates the board stored in Apium's engine
    float evaluate_current_position() const;

    // Evaluate a board given in BoardPos notation
    float evaluate_board_position(std::string& currentBoadPos) const;

    // This version of minimax uses Apium's internal board
    /**
     * Returns a vector of depth-lenght containing depth-number of moves along with the vector's evaluation
     * */
    ApiumLine minimax(int depth, float alpha, float beta, bool maximizingPlayer, std::vector<Move> currentLine = std::vector<Move>());
private:
    float m_eval;
    // Used as a weight to calculate the position's score
    Playstyle m_playstyle;
    // The moves Apium comes up with and actually performs, if the opponent does something expected you don't need to recalculate
    std::vector<Move> m_moves;
    // Used for simulate_functions and access enemy logs
    GameEngine m_engine;
};
