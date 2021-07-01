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
    if (move.type == MOVE) {
        matrix[move.endingCoord.row][move.endingCoord.column].piece =
                matrix[move.startingCoord.row][move.startingCoord.column].piece;
        matrix[move.startingCoord.row][move.startingCoord.column].piece = VUOTA;
    }
    else if (move.type == EAT) {
        // A forward square is where the damina would go is it ate it's target
        std::vector<Square> forwardSquares;

        for (int i = 0; i < move.piecesEaten.size(); i++) {
            int verticalDistance;
            int horizontalDistance;
            if (i == 0) {
                verticalDistance = move.startingCoord.row - move.piecesEaten.at(0).row;
                horizontalDistance = move.startingCoord.column - move.piecesEaten.at(0).column;

                forwardSquares.push_back(matrix[move.piecesEaten.at(0).row - verticalDistance][move.piecesEaten.at(0).column - horizontalDistance]);
            } else {
                verticalDistance = forwardSquares.at(i - 1).coords.row - move.piecesEaten.at(i).row;
                horizontalDistance = forwardSquares.at(i - 1).coords.column - move.piecesEaten.at(i).column;
                forwardSquares.push_back(matrix[move.piecesEaten.at(i).row - verticalDistance]
                [move.piecesEaten.at(i).column - horizontalDistance]);
            }
        }
        // Eat all the target pieces
        for (auto & coord : move.piecesEaten) {
            matrix[coord.row][coord.column].piece = VUOTA;
        }
        // Move the piece to its end square
        matrix[forwardSquares.back().coords.row][forwardSquares.back().coords.column].piece
        = matrix[move.startingCoord.row][move.startingCoord.column].piece;

        matrix[move.startingCoord.row][move.startingCoord.column].piece = VUOTA;

    }
}
