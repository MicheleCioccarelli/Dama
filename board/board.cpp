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

void Board::empty_game_initialization() {
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLUMNS; col++) {
            if (row < 3 && matrix[row][col].color == NERA) {
                matrix[row][col].set_piece(VUOTA, TRASPARENTE);
            } else if (row >= (COLUMNS - 3) && matrix[row][col].color == NERA) {
                matrix[row][col].set_piece(VUOTA, TRASPARENTE);
            }
        }
    }
}

// ====== MOVE EXECUTION ======
void Board::execute_move(Move& move) {
    // Input is assumed as matrix-notation
    Square endingSquare = matrix[move.coords.at(1).row][move.coords.at(1).column];
    Square startingSquare = matrix[move.coords.at(0).row][move.coords.at(0).column];
    int lastIndex = move.coords.size() - 1;

    if (move.type == MOVE) {
        endingSquare.piece = startingSquare.piece;
        startingSquare.piece = Piece(TRASPARENTE, VUOTA);

        matrix[move.coords.at(1).row][move.coords.at(1).column]= endingSquare;
        matrix[move.coords.at(0).row][move.coords.at(0).column] = startingSquare;
    } else if (move.type == EAT) {
        startingSquare = matrix[move.coords[0].row][move.coords[0].column];
        endingSquare = matrix[move.coords.at(lastIndex).row][move.coords.at(lastIndex).column];
        // Eat all the target pieces
        for (int i = 1; i < lastIndex; i++) {
            matrix[move.coords.at(i).row][move.coords.at(i).column].piece = Piece(TRASPARENTE, VUOTA);
        }
        // Move the starting square's piece and null the first square out
        endingSquare.piece = startingSquare.piece;
        startingSquare.piece = Piece(TRASPARENTE, VUOTA);

        matrix[move.coords[0].row][move.coords[0].column].piece = startingSquare.piece;
        matrix[move.coords.at(lastIndex).row][move.coords.at(lastIndex).column].piece = endingSquare.piece;
    }
}

void Board::blow_up(Move& move) {
    edit(Coords(move.blownCoord.column, move.blownCoord.row), Piece(TRASPARENTE, VUOTA));
}

void Board::edit(Coords coords, Piece _piece) {
    matrix[coords.row - 1][coords.column].piece = _piece;
}