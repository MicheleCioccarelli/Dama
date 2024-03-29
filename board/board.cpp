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

void Board::clear() {
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLUMNS; col++) {
            matrix[row][col].piece = Piece(TRASPARENTE, VUOTA);
        }
    }
}

// ======= PIECE INITIALIZATION =======
void Board::standard_game_initialization() {
    // Initialize pieces
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLUMNS; col++) {
            if (row < 3 && matrix[row][col].color == NERA) {
                matrix[row][col].set_piece(DAMA, BIANCO);
            } else if (row >= (COLUMNS - 3) && matrix[row][col].color == NERA) {
                matrix[row][col].set_piece(DAMA, NERO);
            }
        }
    }
}

void Board::promote() {
    for (int col = 0; col < COLUMNS; col++) {
        if (matrix[ROWS - 1][col].piece.type == DAMA && matrix[ROWS - 1][col].piece.color == BIANCO) {
            matrix[ROWS - 1][col].piece.type = DAMONE;
        }
    }

    for (int col = 0; col < COLUMNS; col++) {
        if (matrix[0][col].piece.type == DAMA && matrix[ROWS - 1][col].piece.color == NERO) {
            matrix[0][col].piece.type = DAMONE;
        }
    }
}

void Board::colored_game_initialization() {
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLUMNS; col++) {
            if (matrix[row][col].color == NERA) {
                matrix[row][col].piece = Piece(TRASPARENTE, COLORATA);
            }
        }
    }
}

void Board::damone_game_initialization() {
    // Initialize pieces
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLUMNS; col++) {
            if (row < 3 && matrix[row][col].color == NERA) {
                matrix[row][col].set_piece(DAMONE, BIANCO);
            } else if (row >= (COLUMNS - 3) && matrix[row][col].color == NERA) {
                matrix[row][col].set_piece(DAMONE, NERO);
            }
        }
    }
}

void Board::empty_game_initialization() {}

void Board::execute_move(Move& move) {
    // Input is assumed as matrix-notation
    Square startingSquare = matrix[move.startingCoord.row][move.startingCoord.column];

    if (move.endingCoord.row == ROWS - 1 && move.playerColor == BIANCO && startingSquare.piece.type == DAMA) {
        move.wasPromotion = true;
    } else if (move.endingCoord.row == 0 && move.playerColor == NERO && startingSquare.piece.type == DAMA) {
        move.wasPromotion = true;
    }

    if (move.moveType == EAT) {
        if (move.endingCoord.is_uninitialized()) {
            // If this move has type EAT or wasn't constructed properly
            move.calculate_endingCoord();
        }
        if (move.eatenPieces.empty()) {
            // Eat all the target pieces
            for (Coords &currentCoord: move.eatenCoords) {
                // Save the piece you destroy: used in GameEngine::time_machine()
                move.eatenPieces.push_back(matrix[currentCoord.row][currentCoord.column].piece);
                // Destroy the pieces
                matrix[currentCoord.row][currentCoord.column].piece = Piece(TRASPARENTE, VUOTA);
            }
        } else {
            for (Coords &currentCoord: move.eatenCoords) {
                // Destroy the pieces
                matrix[currentCoord.row][currentCoord.column].piece = Piece(TRASPARENTE, VUOTA);
            }
        }
    }
    // This moves the piece from it's orginal square to its destination, done for both EAT and MOVE type moves
    if (move.wasPromotion) {
        startingSquare.piece.type = DAMONE;
    }
    matrix[move.endingCoord.row][move.endingCoord.column].piece = startingSquare.piece;
    matrix[move.startingCoord.row][move.startingCoord.column].piece = Piece(TRASPARENTE, VUOTA);
}

void Board::blow_up(const Move& move) {
    // Assumes matrix notation
    edit_matrix_notation(Coords(move.blownCoord.column, move.blownCoord.row), Piece(TRASPARENTE, VUOTA));
}

void Board::edit_matrix_notation(Coords coords, Piece _piece) {
    matrix[coords.row][coords.column].piece = _piece;
}

void Board::edit_human_notation(Coords coords, Piece _piece) {
    matrix[coords.row - 1][coords.column].piece = _piece;
}
#pragma clang diagnostic pop