#include "apium.h"

Apium::Apium(GameEngine &engine)
    : m_engine(engine) {}

Apium::Apium(GameEngine &engine, Playstyle playstyle)
    : m_engine(engine), m_playstyle(playstyle) {}

void Apium::set_eval(float eval) {
    m_eval = eval;
}

void Apium::setEngine(const GameEngine &rhs) {
    m_engine = rhs;
}

void Apium::set_playstyle(const Playstyle playstyle) {
    m_playstyle = playstyle;
}

float Apium::get_eval() const { return m_eval; }

void Apium::update_eval() {
    set_eval(evaluate_current_position());
}

float Apium::evaluate_piece(Coords pieceCoords, Piece piece) const {
    // Expects matrix-notation
    float pieceEval = 0;

    if (piece == Piece()) {
        piece = m_engine.board.matrix[pieceCoords.row][pieceCoords.column].piece;
    }

    // There was no piece in the position
    if (piece.type == VUOTA) {
        return pieceEval;
    }

     switch (m_playstyle) {
        case NEUTRAL:
        /* EVALUATION TABLE FOR NEUTRAL
         * DAMINA -> +1   | Increment based on position: +0.25
         * DAMONA -> +3   | Increment based on position: +0.25
         */
            if (piece.type == DAMA) {
                if (piece.color == BIANCO) {
                    // White damina
                    pieceEval++;

                    // Add .25 based on how close it is to being a damone (1 square closer = +.25)
                    pieceEval += pieceCoords.row * .25;
                } else {
                    // Black damina
                    pieceEval--;

                    // Same as above but inverted
                    pieceEval -= (7 - pieceCoords.row) * .25;
                }
            } else {
                pieceEval += 3;
            }
            break;
         case AGGRESSIVE:
         case DEFENSIVE:
             break;
    }
    return pieceEval;
}

float Apium::evaluate_current_position() const {
    float returnValue = 0;
    // Fancy way of skipping white squares coming up
    for (int row = 0; row < ROWS; row++) {
        for (int col = (row % 2); col < COLUMNS; col += 2) {
            returnValue += evaluate_piece(Coords((ColumnNotation)col, row));
        }
    }
    return returnValue;
}

float Apium::evaluate_board_position(std::string &currentBoadPos) const {
    float returnValue = 0;
    std::vector<Square> squares;
    // For some reason the = operator doesn't work
    for (const Square& square : BoardPos::notation_to_squares(currentBoadPos)) {
        squares.push_back(square);
    }

    for (const Square& square : squares) {
        returnValue += evaluate_piece(square.coords, square.piece);
    }
    return returnValue;
}

// For each child of position
ApiumLine Apium::minimax(int depth, float alpha, float beta, bool maximizingPlayer, std::vector<Move> currentLine) {
    if (depth == 0 || m_engine.game_over() != GAME_NOT_OVER) {
        // Return the static evaluation of the current position
        return {currentLine, evaluate_current_position()};
    }
    float eval, maxEval, minEval;

    // If the maximazing player is playing (WHITE)
    if (maximizingPlayer) {
        alpha = MAX_EVAL;
        // For each child of the position
        /**
         * Pick one of your pieces and generate all the moves it could make, then play them
         * one by one and evaluate all the positions, this could be optimized because the moves have to be
         * performed both when they are generated and when they are evaluated
         */
        for (const Square& whitePieceSquare : m_engine.whitePiecesSquares) { // For each white piece on the board
            for (Move& currentMove : m_engine.simulate_piece(whitePieceSquare.coords)) { // generate all of its possible moves
                // Evaluate them and repeat
                m_engine.board.execute_move(currentMove);
                m_engine.refresh_piece_vectors();

                m_engine.render.render_board(m_engine.board, BIANCO, currentMove);
                currentLine.push_back(currentMove);
                eval = minimax(depth - 1, alpha, beta, false, currentLine).get_eval();

                m_engine.undo_move(currentMove);
                currentLine.pop_back();

                maxEval = std::max(maxEval, eval);
                alpha = std::max(alpha, eval);
                if (beta <= alpha) {
                    break; // Might be wrong, you probably need to step out of all the loops, good luck :/
                }
            }
        }
        return {currentLine, maxEval};

    } else {
        // If the minimazing player is playing (BLACK)
        beta = MIN_EVAL;
        for (const Square& blackPieceSquare : m_engine.blackPiecesSquares) { // For each white piece on the board
            for (Move& currentMove : m_engine.simulate_piece(blackPieceSquare.coords)) { // generate all of its possible moves
                // Evaluate them and repeat
                m_engine.board.execute_move(currentMove);
                m_engine.refresh_piece_vectors();
                
                m_engine.render.render_board(m_engine.board, NERO, currentMove);

                currentLine.push_back(currentMove);
                eval = minimax(depth - 1, alpha, beta, true, currentLine).get_eval();

                m_engine.undo_move(currentMove);
                currentLine.pop_back();

                minEval = std::min(minEval, eval);
                beta = std::min(beta, eval);
                if (beta <= alpha) {
                    break;
                }
            }
        }
    }
    return {currentLine, minEval};
}
