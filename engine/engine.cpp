#include "engine.h"
#include "../ui/ui.h"

// ====== GAME ENGINE ======
GameEngine::GameEngine(GameStyle gameStyle, BoardTokens _tokens, SetPieces _pieces,
                       BoardCoords _coords, int rows, int columns)
        : render(_tokens, _pieces, _coords), board(rows, columns) {
    switch (gameStyle) {
        case STANDARD:
            board.standard_game_initialization();
            break;
        case DAMONI:
            board.damone_game_initialization();
            break;
        case COLORED:
            board.colored_game_initialization();
            break;
        case EMPTY:
            board.empty_game_initialization();
            break;
    }
}

PlayerColor GameEngine::deduce_color(const Move &move) {
    Move temp(BIANCO);

    if (move.coords.size() == 0) {
        return TRASPARENTE;
    }

    temp.coords.push_back(convert_coords(move.coords[0]));

    switch (board.matrix[temp.coords[0].row][temp.coords[0].column].piece) {
        case DAMA_N:
            return NERO;
        case DAMONE_N:
            return NERO;
        case DAMA_B:
            return BIANCO;
        case DAMONE_B:
            return BIANCO;
        default:
            return TRASPARENTE;
    }
}

Coords GameEngine::convert_coords(Coords coords) {
    return Coords(coords.column, coords.row - 1);
}

void GameEngine::dispatch_move(const Move& move, const bool isBlown) {
    // Add the move to the respective player
    if (move.color == BIANCO) {
        whitePlayer.add_move(move);
    } else if (move.color == NERO) {
        blackPlayer.add_move(move);
    }

    if (isBlown == true) {
        board.blow_up((Move&) move);
    }
    board.execute_move((Move&) move);
}

MoveReturn GameEngine::check_move(Move &move) {
    Piece startingPiece;
    Piece endingPiece;
    Square endingSquare(Coords(Z, 10), NERA);
    Square startingSquare(Coords(Z, 10), NERA);
    int lastIndex = move.coords.size() - 1;
    int horizontalDistance;
    int verticalDistance;

    Coords startingCoords = convert_coords(move.coords[0]);
    Coords endingCoords = convert_coords(move.coords[lastIndex]);

    if (startingCoords.column >= Z || startingCoords.column < A) {
        return OUT_OF_BOUNDS;
    } else if (endingCoords.column >= Z || endingCoords.column < A) {
        return OUT_OF_BOUNDS;
    } else if (startingCoords.row > 7 || startingCoords.row < 0) {
        return OUT_OF_BOUNDS;
    } else if (endingCoords.row > 7 || endingCoords.row < 0) {
        return OUT_OF_BOUNDS;
    }

    startingPiece = board.matrix[startingCoords.row][startingCoords.column].piece;
    endingPiece = board.matrix[endingCoords.row][endingCoords.column].piece;
    endingSquare = board.matrix[endingCoords.row][endingCoords.column];
    startingSquare = board.matrix[startingCoords.row][startingCoords.column];

    // Distance between the starting square and the ening square
    verticalDistance = startingSquare.coords.row - endingSquare.coords.row;
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

    Coords startingCoords;
    Coords endingCoords;

    Piece startingPiece;
    for (int i = 1; i < move.coords.size(); i++) {
        if (i == 1) {
            if (move.coords.at(0).row - 1 < 0 || move.coords.at(0).row - 1 > 7) {
                return OUT_OF_BOUNDS;
            } else if (move.coords.at(1).row - 1 <= 0 || move.coords.at(1).row - 1 >= 7) {
                    return OUT_OF_BOUNDS;
            } else if (move.coords.at(0).column < 0 || move.coords.at(0).column > 7) {
                return OUT_OF_BOUNDS;
            } else if (move.coords.at(1).column <= 0 || move.coords.at(1).column >= 7) {
                return OUT_OF_BOUNDS;
            }
            startingPiece = board.matrix[move.coords.at(0).row - 1][move.coords.at(0).column].piece;

            startingCoords = convert_coords(move.coords[0]);
            endingCoords = convert_coords(move.coords[1]);

            endingSquare = board.matrix[endingCoords.row][endingCoords.column];
            startingSquare = board.matrix[startingCoords.row][startingCoords.column];

            verticalDistance = startingSquare.coords.row - endingSquare.coords.row;
            horizontalDistance = startingSquare.coords.column - endingSquare.coords.column;

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
            Move moveToValidate(move.color, move.type.moveType);
            moveToValidate.add_coords(move.coords[0]);
            moveToValidate.add_coords(move.coords[1]);

            if (check_move(moveToValidate) == POPULATED) {
                // Check if there is an empty space behind the targeted square
                if (forwardSquare.piece == VUOTA) {
                    returnValue = VALID;
                }
            } else {
                // The targeted square doesn't have anything on it
                return EMPTY_TARGET;
            }
        } else if (returnValue == VALID) {
            if (move.coords.at(i).row - 1 <= 0 || move.coords.at(i).row - 1 >= 7) {
                return OUT_OF_BOUNDS;
            } else if (move.coords.at(i).column <= 0 || move.coords.at(i).column >= 7) {
                return OUT_OF_BOUNDS;
            }
            endingCoords = convert_coords(move.coords[i]);

            endingSquare = board.matrix[endingCoords.row][endingCoords.column];
            startingSquare = forwardSquare;
            startingSquare.piece = startingPiece;

            verticalDistance = startingSquare.coords.row - endingSquare.coords.row;
            horizontalDistance = startingSquare.coords.column - endingSquare.coords.column;
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

            Move moveToValidate(move.color, move.type.moveType);
            board.matrix[startingSquare.coords.row][startingSquare.coords.column].piece =
                    startingPiece;
            moveToValidate.add_coords(Coords(startingSquare.coords.column, startingSquare.coords.row + 1));

            // Square to eat
            moveToValidate.add_coords(move.coords[i]);
            if (check_move((Move &) moveToValidate) == POPULATED) {
                // Check if there is an empty space behind the targeted square
                if (forwardSquare.piece == VUOTA) {
                    board.matrix[startingSquare.coords.row][startingSquare.coords.column].piece = VUOTA;
                    returnValue = VALID;
                }
            } else {
                // The targeted square doesn't have anythin on it
                board.matrix[startingSquare.coords.row][startingSquare.coords.column].piece = VUOTA;
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

    PlayerColor currentPlayer = deduce_color(move);

    if (whitePlayer.moves.size() == 0) {
        return ROCK_SOLID;
    }

    if (currentPlayer == BIANCO) {
        if (whitePlayer.moves[whitePlayer.moves.size() - 1].type.moveType == EAT) {
            return ROCK_SOLID;
        }
    } else if (currentPlayer == NERO) {
        if (blackPlayer.moves[blackPlayer.moves.size() - 1].type.moveType == EAT) {
            return ROCK_SOLID;
        }
    }

    if (move.coords.at(0).row - 1 < 0 || move.coords.at(0).row - 1 > 7) {
        return OUT_OF_BOUNDS;
    } else if (move.coords.at(1).row - 1 <= 0 || move.coords.at(1).row - 1 >= 7) {
        return OUT_OF_BOUNDS;
    } else if (move.coords.at(0).column < 0 || move.coords.at(0).column > 7) {
        return OUT_OF_BOUNDS;
    } else if (move.coords.at(1).column <= 0 || move.coords.at(1).column >= 7) {
        return OUT_OF_BOUNDS;
    }
    Coords startingCoords = convert_coords(_startingCoords);
    Coords endingCoords = convert_coords(_endingCoords);

    Square endingSquare = board.matrix[endingCoords.row][endingCoords.column];
    Square startingSquare = board.matrix[startingCoords.row][startingCoords.column];



    // Check if the move is blowable
    if (check_eat((Move&) move) == VALID) {
        return BLOWABLE;
    }
    return ROCK_SOLID;
}

int GameEngine::count_pieces(PlayerColor pColor, int rows, int columns) {
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
    bool isBlown;

    if (move.type.moveReturn != VALID) {
        if (move.type.moveReturn != BLOWABLE) {
            UI::log_error(move.type.moveReturn);
            return move.type.moveReturn;
        }
    }
        switch (move.type.moveType) {
            case MOVE:
                status = check_move((Move &) move);
                break;
            case EAT:
                status = check_eat((Move &) move);
                break;
            case UNINITIALIZED:
                status = UNDEFINED;
        }

        if (move.type.moveReturn == BLOWABLE) {
            isBlown = true;
        }

        if (status == VALID) {
            dispatch_move(move, isBlown);
        }

        UI::log_error(status);
        return status;
}

void GameEngine::promote(int rows, int columns) {
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

bool GameEngine::simulate_damina(Piece piece, Coords coords) {
    Move move(TRASPARENTE);
    ColumnNotation col = coords.column;
    int row = coords.row;
    // These coords are for testing purposes
    Coords tempCoords;

    /*
     * For the damine we check what moves are in front of them
     * (add 1 to the row for white and leave black because check_eat already subtracts 1)
     * The damone are also checked behind
     * */
    switch (piece) {
        case DAMA_B:
            move = Move(Coords((ColumnNotation) col, row),
                        Coords((ColumnNotation) (col - 1), row + 1),
                        BIANCO, MOVE);
            if (check_move(move) == VALID) {
                return true;
            }
            move = Move(Coords((ColumnNotation) col, row),
                        Coords((ColumnNotation)(col + 1), row + 1),
                        BIANCO, MOVE);
            if (check_move(move) == VALID) {
                return true;
            }
            move = Move(Coords((ColumnNotation) col, row),
                        Coords((ColumnNotation) (col - 1), row + 1),
                        BIANCO, EAT);
            if (check_eat(move) == VALID) {
                return true;
            }
            move = Move(Coords((ColumnNotation) col, row),
                        Coords((ColumnNotation) (col + 1), row + 1),
                        BIANCO, EAT);
            if (check_eat(move) == VALID) {
                return true;
            }

        case DAMA_N:
            move = Move(Coords((ColumnNotation) col, row),
                        Coords((ColumnNotation) (col + 1), row - 1),
                        BIANCO, MOVE);
            if (check_move(move) == VALID) {
                return true;
            }
            move = Move(Coords((ColumnNotation) col, row),
                        Coords((ColumnNotation)(col - 1), row - 1),
                        BIANCO, MOVE);
            if (check_move(move) == VALID) {
                return true;
            }
            move = Move(Coords((ColumnNotation) col, row),
                        Coords((ColumnNotation) (col + 1), row - 1),
                        BIANCO, EAT);
            if (check_eat(move) == VALID) {
                return true;
            }
            move = Move(Coords((ColumnNotation) col, row),
                        Coords((ColumnNotation) (col - 1), row - 1),
                        BIANCO, EAT);
            if (check_eat(move) == VALID) {
                return true;
            }
            break;
    }
    return false;
}

bool GameEngine::simulate_damona(Piece piece, Coords coords) {
    Move move(TRASPARENTE);
    ColumnNotation col = coords.column;
    int row = coords.row;

    switch (piece) {
        case DAMONE_B:
            if (simulate_damina(DAMA_B, coords)) {
                return true;
            }

            move = Move(Coords((ColumnNotation) col, row),
                        Coords((ColumnNotation)(col + 1), row - 1),
                        BIANCO, MOVE);
            if (check_move(move) == VALID) {
                return true;
            }
            move = Move(Coords((ColumnNotation) col, row),
                        Coords((ColumnNotation)(col - 1), row - 1),
                        BIANCO, MOVE);
            if (check_move(move) == VALID) {
                return true;
            }
            move = Move(Coords((ColumnNotation) col, row),
                        Coords((ColumnNotation) (col + 1), row - 1),
                        BIANCO, EAT);
            if (check_eat(move) == VALID) {
                return true;
            }
            move = Move(Coords((ColumnNotation) col, row),
                        Coords((ColumnNotation) (col - 1), row - 1),
                        BIANCO, EAT);
            if (check_eat(move) == VALID) {
                return true;
            }
            break;

        case DAMONE_N:
            if (simulate_damina(DAMA_N, coords)) {
                return true;
            }
            move = Move(Coords((ColumnNotation) col, row),
                        Coords((ColumnNotation)(col + 1), row + 1),
                        NERO, MOVE);
            if (check_move(move) == VALID) {
                return true;
            }
            move = Move(Coords((ColumnNotation) col, row),
                        Coords((ColumnNotation)(col - 1), row + 1),
                        NERO, MOVE);
            if (check_move(move) == VALID) {
                return true;
            }
            move = Move(Coords((ColumnNotation) col, row),
                        Coords((ColumnNotation) (col + 1), row + 1),
                        NERO, EAT);
            if (check_eat(move) == VALID) {
                return true;
            }
            move = Move(Coords((ColumnNotation) col, row),
                        Coords((ColumnNotation) (col - 1), row + 1),
                        NERO, EAT);
            if (check_eat(move) == VALID) {
                return true;
            }
            break;
    }
    return false;
}

GameState GameEngine::game_over(int rows, int columns) {
//    if (count_pieces(BIANCO, rows, columns) <= 0) {
//        return BLACK_WIN;
//    } else if (count_pieces(NERO, rows, columns) <= 0) {
//        return WHITE_WIN;
//    }

    // The number of possible moves for each color
    int blackMoves = 0;
    int whiteMoves = 0;

    Move move(TRASPARENTE);

//    for (int row = 0; row < rows - 1; row++) {
//        for (int col = 0; col < columns - 1; col++) {
//    OLD LOOP

    // Look at every piece on the board and see what moves they can do,
    // if none is found then the game is over
    for (int row = rows - 1; row > -1; row--) {
        for (int col = columns - 1; col > -1; col--) {
            switch (board.matrix[row][col].piece) {
                case VUOTA:
                    break;
                case DAMA_N:
                    if (simulate_damina(DAMA_N, Coords((ColumnNotation)col, row+1))) {
                        blackMoves++;
                    }
                    break;
                case DAMA_B:
                    if (simulate_damina(DAMA_B, Coords((ColumnNotation)col, row+1))) {
                        whiteMoves++;
                    }
                    break;
                case DAMONE_B:
                    if (simulate_damona(DAMONE_B, Coords((ColumnNotation)col, row+1))) {
                        whiteMoves++;
                    }
                    break;
                case DAMONE_N:
                    if (simulate_damona(DAMONE_N, Coords((ColumnNotation)col, row+1))) {
                        blackMoves++;
                    }
                    break;
            }
        }
    }
    render.render_board(BIANCO, board, 8, 8);
    if (whiteMoves == 0 && blackMoves >= 0) {
        return BLACK_WIN;
    } else if (blackMoves == 0 && whiteMoves >= 0) {
        return WHITE_WIN;
    } else if (blackMoves == 0 && whiteMoves == 0) {
        return STALEMATE;
    }
    return GOOD;
}
