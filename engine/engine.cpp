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

void GameEngine::dispatch_move(const Move& move) {
    // Add the move to the respective player
    if (move.color == BIANCO) {
        whitePlayer.add_move(move);
    } else if (move.color == NERO) {
        blackPlayer.add_move(move);
    }

    board.execute_move(move);
}

MoveReturn GameEngine::validate_move(Move move) {
    Piece startingPiece = board.matrix[move.piecesEaten.at(0).row][move.piecesEaten.at(0).column].piece;
    Piece endingPiece = board.matrix[move.piecesEaten.at(1).row][move.piecesEaten.at(1).column].piece;

    Square endingSquare = board.matrix[move.piecesEaten.at(1).row][move.piecesEaten.at(1).column];


    // Distance between the starting square and the ening square
    int verticalDistance = move.piecesEaten.at(0).row - move.piecesEaten.at(1).row;
    int horizontalDistance = move.piecesEaten.at(0).column - move.piecesEaten.at(1).column;


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
    // bool is_simple_move = move.coords.size() == 2;

    Square endingSquare = board.matrix[move.piecesEaten.at(1).row][move.piecesEaten.at(1).column];
    Square startingSquare = board.matrix[move.piecesEaten.at(0).row][move.piecesEaten.at(0).column];

    int verticalDistance = move.piecesEaten.at(0).row - move.piecesEaten.at(1).row;
    int horizontalDistance = move.piecesEaten.at(0).column - move.piecesEaten.at(1).column;
    Square forwardSquare = board.matrix[move.piecesEaten.at(1).row - verticalDistance][move.piecesEaten.at(1).column - horizontalDistance];

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
        if (forwardSquare.coords.column < 0 || forwardSquare.coords.column > 8) {
            return OUT_OF_BOUNDS;
        } else if (forwardSquare.coords.row > 8 || forwardSquare.coords.row < 0) {
            return OUT_OF_BOUNDS;
        }
        // Check if there is an empty space behind the targeted square
        if (forwardSquare.piece ==VUOTA) {
            return VALID;
        }
    } else {
        // The targeted square doesn't have anythin on it
        return EMPTY_TARGET;
    }
}

MoveReturn GameEngine::check_blow(Move move) {
    Move lastWhite = whitePlayer.moves.at(whitePlayer.moves.size() - 1);
    Move lastBlack = blackPlayer.moves.at(whitePlayer.moves.size() - 1);

    // Check if your opponent's last move wasn't eatig something
    if (move.color == BIANCO) {
        if (lastBlack.type == EAT || lastBlack.type == BLOW) {
            return INVALID;
        }
    } else if (move.color == NERO) {
        if (lastWhite.type == EAT || lastWhite.type == BLOW) {
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