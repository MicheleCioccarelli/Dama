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

MoveReturn GameEngine::validate_move(Move &move) {
    Piece startingPiece;
    Piece endingPiece;
    Square endingSquare(Coords(Z, 10), NERA, VUOTA);
    Square startingSquare(Coords(Z, 10), NERA, VUOTA);
    int lastIndex = move.coords.size() - 1;
    int horizontalDistance;
    int verticalDistance;

    if (move.type == EAT) {
        startingPiece = board.matrix[move.coords.at(0).row][move.coords.at(0).column].piece;
        endingPiece = board.matrix[move.coords.at(lastIndex).row][move.coords.at(lastIndex).column].piece;
        endingSquare = board.matrix[move.coords.at(lastIndex).row][move.coords.at(lastIndex).column];
        startingSquare = board.matrix[move.coords.at(0).row][move.coords.at(0).column];
    } else {
        startingPiece = board.matrix[move.coords.at(0).row - 1][move.coords.at(0).column].piece;
        endingPiece = board.matrix[move.coords.at(lastIndex).row - 1][move.coords.at(lastIndex).column].piece;
        endingSquare = board.matrix[move.coords.at(lastIndex).row - 1][move.coords.at(lastIndex).column];
        startingSquare = board.matrix[move.coords.at(0).row - 1][move.coords.at(0).column];
    }

    // Distance between the starting square and the ening square
    if (move.type == EAT) {
        verticalDistance = move.coords.at(0).row - move.coords.at(lastIndex).row;
    } else {
        verticalDistance = startingSquare.coords.row - endingSquare.coords.row;
    }
    horizontalDistance = startingSquare.coords.column - endingSquare.coords.column;


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

MoveReturn GameEngine::check_eat(Move& move) {
    if (move.type == MOVE) {
        return INVALID;
    }

    MoveReturn returnValue = UNDEFINED;
    Square endingSquare(Coords(Z, 9), NERA);
    Square startingSquare(Coords(Z, 9), NERA);
    Square forwardSquare(Coords(Z, 9), NERA);
    int verticalDistance;
    int horizontalDistance;

    Piece startingPiece = board.matrix[move.coords.at(0).row - 1][move.coords.at(0).column].piece;
    for (int i = 1; i < move.coords.size(); i++) {
        if (i == 1) {
            endingSquare = board.matrix[move.coords.at(1).row - 1][move.coords.at(1).column];
            startingSquare = board.matrix[move.coords.at(0).row - 1][move.coords.at(0).column];

            verticalDistance = startingSquare.coords.row - endingSquare.coords.row;
            horizontalDistance = startingSquare.coords.column - endingSquare.coords.column;

            if (endingSquare.coords.row <= 0 || endingSquare.coords.row >= 7) {
                return OUT_OF_BOUNDS;
            } else if (endingSquare.coords.column < 1 || endingSquare.coords.column >= 8) {
                return OUT_OF_BOUNDS;
            }

            forwardSquare = board.matrix[endingSquare.coords.row - verticalDistance]
                    [endingSquare.coords.column - horizontalDistance];
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
            } else if (startingSquare.piece == DAMONE_B && endingSquare.piece == DAMA_B) {
                return FRIENDLY_FIRE;
            } else if (startingSquare.piece == DAMONE_N && endingSquare.piece == DAMA_N) {
                return FRIENDLY_FIRE;
            }

            // Create a temporary move to check
            Move moveToValidate(move.color, move.type);
            moveToValidate.add_coords(startingSquare.coords);
            moveToValidate.add_coords(endingSquare.coords);

            if (validate_move(moveToValidate) == POPULATED) {
                // Check if there is an empty space behind the targeted square
                if (forwardSquare.piece == VUOTA) {
                    returnValue = VALID;
                }
            } else {
                // The targeted square doesn't have anything on it
                return EMPTY_TARGET;
            }
        } else if (returnValue == VALID) {
            endingSquare = board.matrix[move.coords.at(i).row - 1][move.coords.at(i).column];
            startingSquare = forwardSquare;
            startingSquare.piece = startingPiece;

            verticalDistance = startingSquare.coords.row - endingSquare.coords.row;
            horizontalDistance = startingSquare.coords.column - endingSquare.coords.column;

            if (endingSquare.coords.row <= 0 || endingSquare.coords.row >= 8) {
                return OUT_OF_BOUNDS;
            } else if (endingSquare.coords.column <= 1 || endingSquare.coords.row >= 8) {
                return OUT_OF_BOUNDS;
            }

            forwardSquare = board.matrix[endingSquare.coords.row - verticalDistance]
                    [endingSquare.coords.column - horizontalDistance];

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

            Move moveToValidate(move.color, move.type);
            board.matrix[startingSquare.coords.row][startingSquare.coords.column].piece =
                    board.matrix[move.coords[0].row - 1][move.coords[0].column].piece;
            moveToValidate.add_coords(startingSquare.coords);
            moveToValidate.add_coords(endingSquare.coords);

            if (validate_move((Move&) moveToValidate) == POPULATED) {
                // Check if there is an empty space behind the targeted square
                if (forwardSquare.piece == VUOTA) {
                    board.matrix[startingSquare.coords.row][startingSquare.coords.column].piece = VUOTA;
                    returnValue = VALID;
                }
            } else {
                // The targeted square doesn't have anythin on it
                return EMPTY_TARGET;
            }
        } if (returnValue == UNDEFINED) {
            return returnValue;
        }
    }

    board.matrix[startingSquare.coords.row][startingSquare.coords.column].piece = startingPiece;
    move.coords.emplace_back(Coords(forwardSquare.coords.column, forwardSquare.coords.row + 1));
    return returnValue;
}

// You give 2 coords, then you construct a trasparent move with those coords and you check it
MoveReturn GameEngine::check_blow(Coords _startingCoords, Coords _endingCoords) {
    Move move = Move(_startingCoords, _endingCoords);
    Square endingSquare = board.matrix[move.coords.at(1).row - 1][move.coords.at(1).column];
    Square startingSquare = board.matrix[move.coords.at(0).row - 1][move.coords.at(0).column];

    if (startingSquare.piece == DAMONE_B && endingSquare.piece == DAMA_N) {
        return ROCK_SOLID;
    } else if (startingSquare.piece == DAMONE_N && endingSquare.piece == DAMA_B) {
        return ROCK_SOLID;
    }
    // Check if the move is blowable
    if (check_eat((Move&) move) == VALID) {
        return BLOWABLE;
    }
    return ROCK_SOLID;
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

MoveReturn GameEngine::submit(const Move& move) {
    MoveReturn status;

    switch (move.type) {
        case MOVE:
            status = validate_move((Move&)move);
            break;
        case EAT:
            status = check_eat((Move&)move);
            break;
    }

    if (status == VALID) {
        dispatch_move(move);
    }
    return status;
}

void GameEngine::promote() {
    for (int row = 0; row <= rows; row += 7) {
        for (int col = 0; col < columns; col++) {
            if (board.matrix[row][col].piece == DAMA_N && row == 0) {
                board.matrix[row][col].piece = DAMONE_N;
            } else if (row == 7 && board.matrix[row][col].piece == DAMA_B) {
                board.matrix[row][col].piece = DAMONE_B;
            }
        }
    }
}