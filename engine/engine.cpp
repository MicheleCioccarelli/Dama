#include "engine.h"

// ====== GAME ENGINE ======
GameEngine::GameEngine(GameStyle gameStyle, BoardTokens _tokens, SetPieces _pieces, BoardCoords _coords)
        : render(_tokens, _pieces, _coords) {
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
    // Add the move to the respective player
    if (move.color == BIANCO) {
        whitePlayer.add_move(move);
    } else if (move.color == NERO) {
        blackPlayer.add_move(move);
    }

    board.execute_move(move);
}

MoveReturn GameEngine::validate_move(Move move) {
    Piece startingPiece = board.matrix[move.startingCoord.row][move.startingCoord.column].piece;
    Piece endingPiece = board.matrix[move.endingCoord.row][move.endingCoord.column].piece;

    Square endingSquare = board.matrix[move.endingCoord.row][move.endingCoord.column];


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

MoveReturn GameEngine::check_eat(Move move) {
    Square endingSquare = board.matrix[move.endingCoord.row][move.endingCoord.column];
    Square startingSquare = board.matrix[move.startingCoord.row][move.startingCoord.column];

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
        if (board.matrix[move.endingCoord.row - verticalDistance][move.endingCoord.column - horizontalDistance].piece ==
            VUOTA) {
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

MoveReturn GameEngine::check_blow(Move move) {
    PlayerColor playerColor = move.color;

    // Check if your opponent's last move wasn't eatig something
    if (playerColor == BIANCO) {
        if (whitePlayer.moves.at(whitePlayer.moves.size() - 1).type == EAT ||
            whitePlayer.moves.at(whitePlayer.moves.size() - 1).type == BLOW) {
            return INVALID;
        }
    } else if (playerColor == NERO) {
        if (blackPlayer.moves.at(blackPlayer.moves.size() - 1).type == EAT ||
            blackPlayer.moves.at(blackPlayer.moves.size() - 1).type == BLOW) {
            return INVALID;
        }
    }/*
    board.execute_move(Move(Coords(move.endingCoord.column, move.endingCoord.row),
                            Coords(move.startingCoord.column, move.startingCoord.row)));*/
}

int GameEngine::count_pieces(PlayerColor pColor) {
    int returnValue = 0;
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {
            if (pColor == BIANCO) {
                if (board.matrix[row][col].piece == DAMA_B || board.matrix[row][col].piece == DAMONE_B)
                    returnValue++;
            } else if (pColor == NERO) {
                if (board.matrix[row][col].piece == DAMA_N || board.matrix[row][col].piece == DAMONE_N)
                    returnValue++;
            }
        }
    }
    return returnValue;
}

MoveReturn GameEngine::submit(Move move) {
    MoveReturn status;

    switch (move.type) {
        case MOVE:
            status = validate_move(move);
            break;
        case EAT:
            status = check_eat(move);
            break;
        case BLOW:
            status = check_blow(move);
            break;
    }

    if (status == VALID) {
        dispatch_move(move);
    }
    return status;
}