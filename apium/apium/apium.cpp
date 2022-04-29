#include "apium.h"

float Apium::evaluate_piece(Coords pieceCoords) {
    // Expects matrix-notation
    float pieceEval = 0;
    Piece tempPiece = engine.board.matrix[pieceCoords.row][pieceCoords.column].m_piece;

    // There was no piece in the position
    if (tempPiece.m_type == VUOTA) {
        return pieceEval;
    }

     switch (playstyle) {
        case NEUTRAL:
        // EVALUATION TABLE FOR NEUTRAL
        // DAMINA -> +1   | Increment based on position: +0.25
        // DAMONA -> +3   | Increment based on position: +0.25
            if (tempPiece.m_type == DAMA) {
                if (tempPiece.m_color == BIANCO) {
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

float Apium::get_eval() const { return eval; }

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
