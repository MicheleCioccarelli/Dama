#include "apium.h"

Apium::Apium(GameEngine &engine) {
    m_engine = engine;
}

void Apium::set_eval(float eval) {
    m_eval = eval;
}

float Apium::evaluate_piece(Coords pieceCoords, Piece piece) {
    // Expects matrix-notation
    float pieceEval = 0;

    if (piece == Piece()) {
        piece = m_engine.board.matrix[pieceCoords.row][pieceCoords.column].m_piece;
    }

    // There was no piece in the position
    if (piece.type == VUOTA) {
        return pieceEval;
    }

     switch (m_playstyle) {
        case NEUTRAL:
        // EVALUATION TABLE FOR NEUTRAL
        // DAMINA -> +1   | Increment based on position: +0.25
        // DAMONA -> +3   | Increment based on position: +0.25
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

float Apium::get_eval() const { return m_eval; }

float Apium::evaluate_current_position() {
    float returnValue = 0;
    // Fancy way of skipping white squares coming up
    for (int row = 0; row < ROWS; row++) {
        for (int col = (row % 2); col < COLUMNS; col += 2) {
            returnValue += evaluate_piece(Coords((ColumnNotation)col, row));
        }
    }
    return returnValue;
}

float Apium::evaluate_board_position(std::string &currentBoadPos) {
    float returnValue = 0;
    std::vector<Square> squares;
    // For some reason the = operator doesn't work
    for (Square square : BoardPos::notation_to_squares(currentBoadPos)) {
        squares.push_back(square);
    }

    for (Square square : squares) {
        returnValue += evaluate_piece(square.m_coords, square.m_piece);
    }
    return returnValue;
}
