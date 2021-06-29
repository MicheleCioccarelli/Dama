#include "engine.h"

// ====== GAME ENGINE ======
GameEngine::GameEngine() {
    board.standard_game_initialization();
}

void GameEngine::dispatch_move(Move move) {
    if (move.piece == DAMA_B || move.piece == DAMONE_B) {
        whitePlayer.add_move(move);
    } else {
        blackPlayer.add_move(move);
    }
    board.execute_move(move);
}

int GameEngine::count_pieces(PlayerColor pColor) {
    int returnValue = 0;
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {
            if(pColor == BIANCO) {
                if (board.board[row][col].piece == DAMA_B || board.board[row][col].piece == DAMONE_B)
                    returnValue++;
            } else if (pColor == NERO) {
                if (board.board[row][col].piece == DAMA_N || board.board[row][col].piece == DAMONE_N)
                    returnValue++;
            }
        }
    }
    return returnValue;
}
