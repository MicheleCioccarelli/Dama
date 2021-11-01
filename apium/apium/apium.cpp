#include "apium.h"

Apium::Apium(Board &board, GameEngine& engine) {
    this->engine = engine;
    // Fill up pieceCoords
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLUMNS; col++) {
            if (board.matrix[row][col].piece.type != VUOTA) {
                Square square = board.matrix[row][col];
                pieceCoords.push_back(square);
            }
        }
    }
}

void Apium::evaluate_moves() {
    // Work for tomorrow :) (or the day after that)
}
