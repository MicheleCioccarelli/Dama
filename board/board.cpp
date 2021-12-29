#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
#include "board.h"

Board::Board() {
    // Initialize vector
    matrix.resize(ROWS);

    // Give color and coords to every square
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLUMNS; col++) {
            Coords currentCoords = Coords((ColumnNotation) col, row);
            if ((col + row) % 2 == 0) {
                matrix[row].emplace_back(currentCoords, NERA);
            } else {
                matrix[row].emplace_back(currentCoords, BIANCA);
            }
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
    if (move.type == EAT) {
        if (move.endingCoord.is_uninitialized()) {
            // If this move has type EAT or wasn't constructed properly
            move.calculate_endingCoord();
        }
        Square endingSquare = matrix[move.endingCoord.row][move.endingCoord.column];

        // Eat all the target pieces
        for (int i = 0; i < move.eatenCoords.size(); i++) {
            // Save the piece you destroy: used in GameEngine::time_machine()
            move.eatenPieces.push_back(matrix[move.eatenCoords.at(i).row][move.eatenCoords.at(i).column].piece);
            // Destroy the pieces
            matrix[move.eatenCoords.at(i).row][move.eatenCoords.at(i).column].piece = Piece(TRASPARENTE, VUOTA);
        }
    }
    // This moves the piece from it's orginal square to its destination, done for both EAT and MOVE type moves
    matrix[move.endingCoord.row][move.endingCoord.column].piece = startingSquare.piece;
    matrix[move.startingCoord.row][move.startingCoord.column].piece = Piece(TRASPARENTE, VUOTA);
}

void Board::blow_up(Move& move) {
    edit_human_notation(Coords(move.blownCoord.column, move.blownCoord.row), Piece(TRASPARENTE, VUOTA));
}

void Board::edit_matrix_notation(Coords coords, Piece _piece) {
    matrix[coords.row][coords.column].piece = _piece;
}

void Board::edit_human_notation(Coords coords, Piece _piece) {
    matrix[coords.row - 1][coords.column].piece = _piece;
}
#pragma clang diagnostic pop