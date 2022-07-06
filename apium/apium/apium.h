#pragma once

#include "../apiumline/apiumline.h"
#include "../../boardPos/boardPos.h"
#include "../../engine/engine.h"
#include "../a_piece/a_piece.h"
#include "../../enums/playstyle.h"

#define DEBUG

class Apium {
public:
    Apium() = default;
    Apium(GameEngine& engine);
    Apium(GameEngine& engine, Playstyle playstyle, PlayerColor apiumColor);
    Apium(Playstyle playStyle, PlayerColor apiumColor);

    float get_eval() const;
    void set_eval (float eval);

    // Copy the rhs engine into Apium's engine
    void setEngine(const GameEngine& rhs);

    void set_playstyle(Playstyle playstyle);

    /**
    * plays a depth number of moves and decides the best combination, or line.
    * For example with depth 3 it would play 1 move on Apiums color, one move from the opponent,
    * and one move back as Apium\n\n
    * This is me giving up on fancy algorithms
    * */
    ApiumLine find_best_line(int depth);
private:
    // Runs an evaluation of the current position and evaluates the internal evaluation
    void update_eval();
    /** Returns the value of the piece (positive for white pieces and negative for black ones).
    * The if piece is not provided, the one in m_engine.board.matrix at the coordinates provided is used
    */
    float evaluate_piece(Coords pieceCoords, Piece piece = Piece()) const;

    // Evaluates the board stored in Apium's engine
    float evaluate_current_position() const;

    // Evaluate a board given in BoardPos notation
    float evaluate_board_position(std::string& currentBoadPos) const;

    // This version of minimax uses Apium's internal board
    /**
     * Returns a vector of depth-lenght containing depth-number of moves along with the vector's evaluation
     * */
    ApiumLine minimax(int depth, float alpha, float beta, bool maximizingPlayer, std::vector<Move> currentLine = std::vector<Move>());
    /**
     * Given the current board finds the best move for the color specified wthout looking ahead
     * if shouldCleanup is true the function automatically undoes the moves it makes, if it is false
     * cleanup is a responsability of the caller
     * */
    Move find_best_move(PlayerColor whoIsPlaying, bool shouldCleanup = true);

private:
    PlayerColor whoIsBeingPlayed = BIANCO;

    float m_eval = 0;
    // Used as a weight to calculate the position's score
    Playstyle m_playstyle;
    // The moves Apium comes up with and actually performs, if the opponent does something expected you don't need to recalculate
    std::vector<Move> m_moves;
    // Used for simulate_functions and access enemy logs
    GameEngine m_engine;
public:
    ApiumLine bestLine;
};
