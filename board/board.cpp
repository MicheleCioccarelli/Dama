#include "board.h"

Board::Board() {
    // Initialize vector
    board.resize(rows);

    // Give color and coords to every square
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {
            Coords currentCoords = Coords((ColumnNotation) row, col);

            if ((col + row) % 2 == 0 ) {
                board[row].push_back(Square(currentCoords, NERA));
            } else {
                board[row].push_back(Square(currentCoords, BIANCA));
            }
        }
    }
}

void Board::standard_game_initialization() {
    // Initialize pieces
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {
            if (row < 3 && board[row][col].color == NERA) {
                board[row][col].set_piece(DAMA_B);
            } else if (row > 4 && board[row][col].color == NERA) {
                board[row][col].set_piece(DAMA_N);
            }
        }
    }
}

void Board::render() {

}