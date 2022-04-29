#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
#include "board.h"

Board::Board() {
    // Give color and coords to every square
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLUMNS; col++) {
            Coords currentCoords = Coords((ColumnNotation) col, row);
            if ((col + row) % 2 == 0) {
                matrix[row][col] = Square(currentCoords, NERA);
            } else {
                matrix[row][col] = Square(currentCoords, BIANCA);
            }
        }
    }
}

// ======= PIECE INITIALIZATION =======
void Board::standard_game_initialization() {
    // Initialize pieces
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLUMNS; col++) {
            if (row < 3 && matrix[row][col].m_color == NERA) {
                matrix[row][col].set_piece(DAMA, BIANCO);
            } else if (row >= (COLUMNS - 3) && matrix[row][col].m_color == NERA) {
                matrix[row][col].set_piece(DAMA, NERO);
            }
        }
    }
}

void Board::colored_game_initialization() {
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLUMNS; col++) {
            if (matrix[row][col].m_color == NERA) {
                matrix[row][col].m_piece = Piece(TRASPARENTE, COLORATA);
            }
        }
    }
}

void Board::damone_game_initialization() {
    // Initialize pieces
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLUMNS; col++) {
            if (row < 3 && matrix[row][col].m_color == NERA) {
                matrix[row][col].set_piece(DAMONE, BIANCO);
            } else if (row >= (COLUMNS - 3) && matrix[row][col].m_color == NERA) {
                matrix[row][col].set_piece(DAMONE, NERO);
            }
        }
    }
}

void Board::empty_game_initialization() {}

void Board::execute_move(Move& move) {
    // Input is assumed as matrix-notation
    Square startingSquare = matrix[move.startingCoord.row][move.startingCoord.column];
    if (move.moveType == EAT) {
        if (move.endingCoord.is_uninitialized()) {
            // If this move has type EAT or wasn't constructed properly
            move.calculate_endingCoord();
        }
        if (move.endingCoord.row == ROWS - 1 && move.playerColor == BIANCO) {
            move.m_wasPromotion = true;
        } else if (move.endingCoord.row == 0 && move.playerColor == NERO) {
            move.m_wasPromotion = true;
        }
        if (move.eatenPieces.empty()) {
            // Eat all the target pieces
            for (Coords &currentCoord: move.eatenCoords) {
                // Save the piece you destroy: used in GameEngine::time_machine()
                move.eatenPieces.push_back(matrix[currentCoord.row][currentCoord.column].m_piece);
                // Destroy the pieces
                matrix[currentCoord.row][currentCoord.column].m_piece = Piece(TRASPARENTE, VUOTA);
            }
        } else {
            for (Coords &currentCoord: move.eatenCoords) {
                // Destroy the pieces
                matrix[currentCoord.row][currentCoord.column].m_piece = Piece(TRASPARENTE, VUOTA);
            }
        }
    }
    // This moves the piece from it's orginal square to its destination, done for both EAT and MOVE type moves
    if (move.m_wasPromotion) {
        startingSquare.m_piece.m_type = DAMONE;
    }
    matrix[move.endingCoord.row][move.endingCoord.column].m_piece = startingSquare.m_piece;
    matrix[move.startingCoord.row][move.startingCoord.column].m_piece = Piece(TRASPARENTE, VUOTA);
}

void Board::blow_up(Move& move) {
    // Assumes matrix notation
    edit_matrix_notation(Coords(move.blownCoord.column, move.blownCoord.row), Piece(TRASPARENTE, VUOTA));
}

void Board::edit_matrix_notation(Coords coords, Piece _piece) {
    matrix[coords.row][coords.column].m_piece = _piece;
}

void Board::edit_human_notation(Coords coords, Piece _piece) {
    matrix[coords.row - 1][coords.column].m_piece = _piece;
}
#pragma clang diagnostic pop