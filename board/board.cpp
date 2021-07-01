#include "board.h"

Board::Board() {
    // Initialize vector
    matrix.resize(rows);

    // Give color and coords to every square
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {
            Coords currentCoords = Coords((ColumnNotation) row, col);
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
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {
            if (row < 3 && matrix[row][col].color == NERA) {
                matrix[row][col].set_piece(DAMA_B);
            } else if (row >= (columns - 3) && matrix[row][col].color == NERA) {
                matrix[row][col].set_piece(DAMA_N);
            }
        }
    }
}

void Board::colored_game_initialization() {
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {
            if (matrix[row][col].color == NERA) {
                matrix[row][col].piece = COLORATA;
            }
        }
    }
}

void Board::damone_piece_initialization() {
    // Initialize pieces
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {
            if (row < 3 && matrix[row][col].color == NERA) {
                matrix[row][col].set_piece(DAMONE_B);
            } else if (row >= (columns - 3) && matrix[row][col].color == NERA) {
                matrix[row][col].set_piece(DAMONE_N);
            }
        }
    }
}

// ====== MOVE EXECUTION ======
void Board::execute_move(Move move) {
    matrix[move.endingCoord.row][move.endingCoord.column].piece =
            matrix[move.startingCoord.row][move.startingCoord.column].piece;
    matrix[move.startingCoord.row][move.startingCoord.column].piece = VUOTA;
}
