#include "engine.h"

// ====== GAME ENGINE ======
GameEngine::GameEngine(GameStyle gameStyle) {
    switch (gameStyle) {
        case STANDARD:
            board.standard_game_initialization();
            break;
        case DAMONI:
            board.damone_piece_initialization();
            break;
        case COLORED:
            board.colored_game_initialization();
            break;
    }
}

void GameEngine::dispatch_move(Move move) {
    Piece startingPiece = board.board[move.startingCoord.row][move.startingCoord.column].piece;

    if (startingPiece == DAMA_B || startingPiece == DAMONE_B) {
        whitePlayer.add_move(move);
    } else {
        blackPlayer.add_move(move);
    }
    board.execute_move(move);
}

ErrorType GameEngine::validate_move(Move move) {
    Piece startingPiece = board.board[move.startingCoord.row][move.startingCoord.column].piece;
    Piece endingPiece = board.board[move.endingCoord.row][move.endingCoord.column].piece;
    Square endingSquare = board.board[move.endingCoord.row][move.endingCoord.column];

    // Distance between the starting square and the ening square
    int verticalDistance = move.startingCoord.row - move.endingCoord.row;
    int horizontalDistance = move.startingCoord.column - move.endingCoord.column;

    // Check if ou arre actually moving a piece, not just air
    if (startingPiece == VUOTA) {
        return EMPTY_START;
    }
    // Check if you are going in a black square
    if (endingSquare.color != NERA) {
        return WHITE_SQUARE;
    }
    // Check if you are moving by one square
    if (verticalDistance == 1 || verticalDistance == -1) {
        if (horizontalDistance == 1 || horizontalDistance == -1) {
            // Check if a damina is moving forwards
            if (startingPiece == DAMA_B && verticalDistance == 1 ||
                startingPiece == DAMA_N && verticalDistance == -1) {
                return BEHIND;
            }
            if (endingPiece != VUOTA) {
                return POPULATED;
            }
            return VALID;
        }
    }
    return TOO_FAR;
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
