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

PlayerColor GameEngine::determine_player(Move &move) {
    switch (board.board[move.startingCoord.row][move.startingCoord.column].piece) {
        case DAMA_B:
        case DAMONE_B:
            return BIANCO;
        case DAMA_N:
        case DAMONE_N:
            return NERO;
        default:
            return TRASPARENTE;
    }
}

void GameEngine::dispatch_move(Move &move) {
    // Add the move to the respective player
    if (determine_player(move) == BIANCO) {
        whitePlayer.add_move(move);
    } else if (determine_player(move) == NERO) {
        blackPlayer.add_move(move);
    }
    board.execute_move(move);
}

ErrorType GameEngine::validate_move(Move &move) {
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
            move.moveType = MOVE;
                return VALID;
            }
        }
        return TOO_FAR;
}

ErrorType GameEngine::check_eat(Move &move) {
    Square endingSquare = board.board[move.endingCoord.row][move.endingCoord.column];
    Square startingSquare = board.board[move.startingCoord.row][move.startingCoord.column];
    int verticalDistance = move.startingCoord.row - move.endingCoord.row;
    int horizontalDistance = move.startingCoord.column - move.endingCoord.column;

    // Check piece compatibility
    if (startingSquare.piece == endingSquare.piece) {
        return CANNIBALISM;
    } else if (startingSquare.piece == DAMA_B && endingSquare.piece == DAMONE_N) {
        return TOO_BIG;
    } else if (startingSquare.piece == DAMA_N && endingSquare.piece == DAMONE_B) {
        return TOO_BIG;
    } else if (startingSquare.piece == DAMA_B && endingSquare.piece == DAMONE_B) {
        return FRIENDLY_FIRE;
    } else if (startingSquare.piece == DAMA_N && endingSquare.piece == DAMONE_N) {
        return FRIENDLY_FIRE;
    }

    if (validate_move(move) == POPULATED) {
        // Check if there is an empty space behind the targeted square
        if (board.board[move.endingCoord.row - verticalDistance][move.endingCoord.column - horizontalDistance].piece == VUOTA) {
            // Add the move to the respective players
            move.moveType = EAT;
            return VALID;
        } else {
            // There isn't any free space behind the targeted square
            return POPULATED;
        }
    } else {
        // The targeted square doesn't have anythin on it
        return EMPTY_TARGET;
    }
}

ErrorType GameEngine::check_blow(Move &move) {
    PlayerColor playerColor = determine_player(move);

    // Check if your opponent's last move wasn't eatig something
    if (playerColor == BIANCO) {
        if (whitePlayer.moves.at(whitePlayer.moves.size() - 1).moveType == EAT ||
            whitePlayer.moves.at(whitePlayer.moves.size() - 1).moveType == BLOW) {
            return INVALID;
        }
    } else if (playerColor == NERO) {
        if (blackPlayer.moves.at(blackPlayer.moves.size() - 1).moveType == EAT ||
            blackPlayer.moves.at(blackPlayer.moves.size() - 1).moveType == BLOW) {
            return INVALID;
        }
    }
    board.execute_move(Move(Coords(move.endingCoord.column, move.endingCoord.row),
                            Coords(move.startingCoord.column, move.startingCoord.row)));

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
