#include "board.h"

Board::Board() {
    // Initialize vector
    matrix.resize(rows);

    // Give color and coords to every square
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {
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
    Square endingSquare = matrix[move.coords.at(1).row - 1][move.coords.at(1).column];
    Square startingSquare = matrix[move.coords.at(0).row - 1][move.coords.at(0).column];
    int lastIndex = move.coords.size() - 1;

    if (move.type == MOVE) {
        endingSquare.piece = startingSquare.piece;
        startingSquare.piece = VUOTA;

        matrix[move.coords.at(1).row - 1][move.coords.at(1).column]= endingSquare;
        matrix[move.coords.at(0).row - 1][move.coords.at(0).column] = startingSquare;
    } else if (move.type == EAT) {
        startingSquare = matrix[move.coords[0].row - 1][move.coords[0].column];
        endingSquare = matrix[move.coords.at(lastIndex).row - 1][move.coords.at(lastIndex).column];
/*
        for (int i = 1; i <= move.coords.size(); i++) {
            if (i == 1) {
                endingSquare = matrix[move.coords.at(i).row - 1][move.coords.at(i).column];
                startingSquare = matrix[move.coords.at(0).row - 1][move.coords.at(0).column];

                verticalDistance = startingSquare.coords.row - endingSquare.coords.row;
                horizontalDistance = startingSquare.coords.column - endingSquare.coords.column;
                forwardSquare = matrix[endingSquare.coords.row - verticalDistance]
                [endingSquare.coords.column - horizontalDistance];
            } else {
                endingSquare = matrix[move.coords.at(i).row - 1][move.coords.at(i).column];
                startingSquare = forwardSquare;

                verticalDistance = startingSquare.coords.row - endingSquare.coords.row;
                horizontalDistance = startingSquare.coords.column - endingSquare.coords.column;
                forwardSquare = matrix[endingSquare.coords.row - verticalDistance]
                [endingSquare.coords.column - horizontalDistance];
            }
        }*/
        // Eat all the target pieces
        for (int i = 1; i < lastIndex; i++) {
            matrix[move.coords.at(i).row - 1][move.coords.at(i).column].piece = VUOTA;
        }
        // Move the starting square's piece and null the first square out
        endingSquare.piece = startingSquare.piece;
        startingSquare.piece = VUOTA;

        matrix[move.coords[0].row - 1][move.coords[0].column].piece = startingSquare.piece;
        matrix[move.coords.at(lastIndex).row - 1][move.coords.at(lastIndex).column].piece
            = endingSquare.piece;
    }
}
