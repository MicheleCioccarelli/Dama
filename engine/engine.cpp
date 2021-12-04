#include "engine.h"
#include "../ui/ui.h"

// ====== GAME ENGINE ======
GameEngine::GameEngine(GameStyle gameStyle)
        : render(), board() {
    start = time(nullptr);
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

Coords GameEngine::calculate_forward(const Coords &startingCoords, const Coords &endingCoords) {
    int verticalDistance = startingCoords.row - endingCoords.row;
    int horizontalDistance = startingCoords.column - endingCoords.column;

    return {(ColumnNotation)(endingCoords.column - horizontalDistance), endingCoords.row - verticalDistance};
}

PlayerColor GameEngine::deduce_color(Move &move) {
    Move temp(BIANCO);

    if (move.coords.empty()) {
        return TRASPARENTE;
    }

    temp.coords.push_back(move.coords[0].convert_coords());

    return board.matrix[temp.coords[0].row][temp.coords[0].column].piece.color;
}

void GameEngine::dispatch_move(const Move& move, const bool isBlown) {
    // Assumes matrix-notation input

    // Add the move to the respective player
    if (move.playerColor == BIANCO) {
        whitePlayer.add_move(move);
    } else if (move.playerColor == NERO) {
        blackPlayer.add_move(move);
    }

    if (isBlown == true) {
        board.blow_up((Move&) move);
    }
    board.execute_move((Move&) move);

}

bool GameEngine::is_in_bounds(Coords coords) {
    if (coords.row  < 0 || coords.row > 7) {
        return false;
    } else if (coords.column < 0 || coords.column > 7) {
        return false;
    }
    return true;
}

Coords GameEngine::calculate_forward(const Move &move) {
    Coords tempForward = calculate_forward(move.coords[0], move.coords[1]);
    for (int i = 2; i < move.coords.size(); i++) {
        tempForward = calculate_forward(tempForward, move.coords[i]);
    }
    return tempForward;
}

MoveReturn GameEngine::check_move(Move &move) {
    // Has to assume matrix-notation, then fix simulate_damina
    Piece startingPiece;
    Piece endingPiece;
    Square endingSquare(Coords(Z, 10), NERA);
    Square startingSquare(Coords(Z, 10), NERA);
    int lastIndex = move.coords.size() - 1;
    int horizontalDistance;
    int verticalDistance;

    if (!is_in_bounds(move.coords.at(0)) || !is_in_bounds(move.coords.at(1))) {
        return OUT_OF_BOUNDS;
    }

    Coords startingCoords = move.coords[0];
    Coords endingCoords = move.coords[lastIndex];

    startingPiece = board.matrix[startingCoords.row][startingCoords.column].piece;
    endingPiece = board.matrix[endingCoords.row][endingCoords.column].piece;
    endingSquare = board.matrix[endingCoords.row][endingCoords.column];
    startingSquare = board.matrix[startingCoords.row][startingCoords.column];

    // Distance between the starting square and the ening square
    verticalDistance = startingSquare.coords.row - endingSquare.coords.row;
    horizontalDistance = startingSquare.coords.column - endingSquare.coords.column;

    // Check if ou arre actually moving a piece, not just air
    if (startingPiece.type == VUOTA) {
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
            if (startingPiece.type == DAMA && startingPiece.color == BIANCO && verticalDistance == 1 ||
            startingPiece.type == DAMA && startingPiece.color == NERO && verticalDistance == -1) {
                return BEHIND;
            }
            if (endingPiece.type != VUOTA && endingPiece.type != COLORATA) {
                return POPULATED;
            }
            return VALID;
        }
    }
    return TOO_FAR;
}

MoveReturn GameEngine::inspect_dama(Coords startingCoords, Coords endingCoords, bool dirt) {
    Square startingSquare = board.matrix[startingCoords.row][startingCoords.column];
    Square endingSquare = board.matrix[endingCoords.row][endingCoords.column];
    // Check square-specific details
    if (endingSquare.piece.type == VUOTA) {
        return EMPTY_TARGET;
    } else if (startingSquare.piece.type == VUOTA && dirt == false) {
        return EMPTY_START;
    }
    if (startingSquare.color == BIANCA || endingSquare.color == BIANCA) {
        return WHITE_SQUARE;
    }
    // COLORATA is used for debugging purposes and will not show up in a regular game
    if (endingSquare.piece.type != VUOTA && endingSquare.piece.type != COLORATA) {
        return POPULATED;
    }

    // Check pieces compatibility
    if (startingSquare.piece.type == DAMA && endingSquare.piece.type == DAMONE) {
        return TOO_BIG;
    } else if (startingSquare.piece.color == endingSquare.piece.color) {
        return FRIENDLY_FIRE;
    }

    int verticalDistance = startingCoords.row - endingCoords.row;
    int horizontalDistance = startingCoords.column - endingCoords.column;
    // Check if you are moving by one square
    if (verticalDistance == 1 || verticalDistance == -1) {
        if (horizontalDistance == 1 || horizontalDistance == -1) {
            // Check if a damina is moving forwards
            if (startingSquare.piece.type == DAMA && startingSquare.piece.color == BIANCO && verticalDistance == 1 ||
                startingSquare.piece.type == DAMA && startingSquare.piece.color == NERO && verticalDistance == -1) {
                return BEHIND;
            }
            return VALID;
        } else {
            return TOO_FAR;
        }
    } else {
        return TOO_FAR;
    }
}

MoveReturn GameEngine::recursive_check_eat(Move move, Coords startingCoords, int index) {
    // Assumes a move with valid syntax and matrix notation
    /* When you check for multiple eatings the position you start from is actually empty because
     * this function doesn't move pieces around, dirt tells inspect_dama() to ignore the emptiness
     */
    bool dirt = true;
    if (index == 1) {
        // This is the first call as index defaults to 1
        startingCoords = move.coords[0];
        dirt = false;
    } else if (index == move.coords.size()) {
        // You are at the end of the move.coords vector, everything went fine (base case)
        return VALID;
    }
    // This is where the dama would go if it ate endingcoords
    Coords forwardSquare = calculate_forward(startingCoords, move.coords[index]);
    if (is_in_bounds(forwardSquare)) {
        // Check if the move doesn't break the rules of the game
        MoveReturn result = inspect_dama(startingCoords, move.coords[index], dirt);
        // If there is a dama to be eaten
        if (result == POPULATED) {
            return recursive_check_eat(move, forwardSquare, index + 1);
        } else {
            // inspect_dama failed
            return result;
        }
    } else {
        // ForwardSquare is out of bounds
        return OUT_OF_BOUNDS;
    }
}

// You give 2 coords, then you construct a trasparent move with those coords, and you check it
MoveReturn GameEngine::check_blow(Coords _startingCoords, Coords _endingCoords) {
    Move move = Move(_startingCoords, _endingCoords);

    PlayerColor currentPlayer = deduce_color(move);

    if (whitePlayer.moves.empty()) {
        return ROCK_SOLID;
    }
    if (blackPlayer.moves.empty()) {
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
    // Check if the move is blowable
    if (recursive_check_eat((Move&) move) == VALID) {
        return BLOWABLE;
    }
    return ROCK_SOLID;
}

int GameEngine::count_pieces(PlayerColor pColor) {
    int returnValue = 0;
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLUMNS; col++) {
            if (pColor == BIANCO) {
                if (board.matrix[row][col].piece.color == BIANCO)
                    returnValue++;
            } else if (pColor == NERO) {
                if (board.matrix[row][col].piece.color == NERO)
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

    // Create a new move with it's coords converted to matrix notation, readable by check functions
    Move tempMove = move;
    for (Coords &i : tempMove.coords) {
        i = i.convert_coords();
    }

    switch (move.type.moveType) {
        case MOVE:
            status = check_move(tempMove);
            break;
        case EAT:
            status = recursive_check_eat(tempMove);
            break;
        case UNINITIALIZED:
            status = UNDEFINED;
    }
    if (tempMove.type.moveReturn == BLOWABLE) {
        isBlown = true;
    }
    if (status == VALID) {
        // more DIRT
        if (tempMove.type.moveType == EAT) {
            tempMove.add_coords(calculate_forward(tempMove));
        }
        dispatch_move(tempMove, isBlown);
    }
    UI::log_error(status);
    return status;
}

void GameEngine::promote() {
    for (int row = 0; row <= ROWS; row += 7) {
        for (int col = 0; col < COLUMNS; col++) {
            if (board.matrix[row][col].piece.color == NERO && board.matrix[row][col].piece.type == DAMA && row == 0)
                board.matrix[row][col].piece.type = DAMONE;
            else if (board.matrix[row][col].piece.color == BIANCO && board.matrix[row][col].piece.type == DAMA && row == 7) {
                board.matrix[row][col].piece.type = DAMONE;
            }
        }
    }
}

std::vector<Move> GameEngine::branch_damina(Coords startingCoords, PlayerColor color, int verticalOffset, int horizontalOffset) {
    std::vector<Move> movesFound;
    Coords endingCoords = Coords(static_cast<ColumnNotation>(startingCoords.column + horizontalOffset),
                                 startingCoords.row + verticalOffset);
    Move move = Move(startingCoords, endingCoords, color, MOVE);
        if (check_move(move) == VALID) {
            movesFound.push_back(move);
        } else if (recursive_check_eat(move) == VALID){
            move.type = EAT;
            movesFound.push_back(move);
        }
    return movesFound;
}

std::vector<Move> GameEngine::simulate_damina(PlayerColor color, Coords coords) {
    // Input coords are assumed to be in matrix notation
    std::vector<Move> movesFound;
    std::vector<Move> tempMoves;

    /*
     * For the damine we check what moves are in front of them
     * (add 1 to the row for white and leave black because check_eat already subtracts 1)
     * The damone are also checked behind
     * */
    // Used when checking if you can eat, increased ever while loop, so you can check for eating multiple pieces
    int verticalOffset;
    int horizontalOffset;

    switch (color) {
        case BIANCO:
            tempMoves = branch_damina(coords, BIANCO, 1, -1);
            // Save the moves found, if any
            if (!tempMoves.empty()) {
                for (const auto &i: tempMoves) {
                    movesFound.push_back(i);
                }
            }
            tempMoves = branch_damina(coords, BIANCO, 1, 1);
            if (!tempMoves.empty()) {
                for (const auto &i: tempMoves) {
                    movesFound.push_back(i);
                }
            }
            break;
        case NERO:
            tempMoves = branch_damina(coords, NERO, -1, 1);
            if (!tempMoves.empty()) {
                for (const auto &i: tempMoves) {
                    movesFound.push_back(i);
                }
            }
            tempMoves = branch_damina(coords, NERO, -1, -1);
            if (!tempMoves.empty()) {
                for (const auto &i: tempMoves) {
                    movesFound.push_back(i);
                }
            }
            break;
    }
    return movesFound;
}

std::vector<Move> GameEngine::simulate_damona(Coords coords) {
    // This move is constantly changed, it simulates a movement
    Move move(TRASPARENTE);

    std::vector<Move> movesFound;
    std::vector<Move> tempMoves;

    tempMoves = simulate_damina(BIANCO, coords);
    if (!tempMoves.empty()) {
        for (const auto &i: tempMoves) {
            movesFound.push_back(i);
        }
    }
    tempMoves = simulate_damina(NERO, coords);
    if (!tempMoves.empty()) {
        for (const auto &i: tempMoves) {
            movesFound.push_back(i);
        }
    }
        return movesFound;
}

void GameEngine::resign(Move& move) {
    int whitePieces = count_pieces(BIANCO);
    int blackPieces = count_pieces(NERO);

    if (move.type.moveReturn == WHITE_RESIGN) {
        RenderV2::end_screen(whitePieces, blackPieces, whitePlayer, blackPlayer, WHITE_RESIGNED, start);
    } else if (move.type.moveReturn == BLACK_RESIGN) {
        RenderV2::end_screen(whitePieces, blackPieces, whitePlayer, blackPlayer, BLACK_RESIGNED, start);
    }
}

GameState GameEngine::game_over(PlayerColor winner) {
    if (winner != TRASPARENTE) {
        if (winner == BIANCO) {
            return WHITE_WIN;
        } else if (winner == NERO) {
            return BLACK_WIN;
        }
    }

    // The number of possible moves for each color
    int blackMoves = 0;
    int whiteMoves = 0;

    Move move(TRASPARENTE);

    // Look at every piece on the board and see what moves they can do,
    // if none is found then the game is over
    for (int row = 0; row <= ROWS - 1; row++) {
        for (int col = 0; col <= COLUMNS - 1; col++) {
            // Coords are already using matrix notation
            switch (board.matrix[row][col].piece.type) {
                case VUOTA:
                    break;
                case DAMA:
                    switch (board.matrix[row][col].piece.color) {
                        case BIANCO:
                            if (!simulate_damina(BIANCO,Coords((ColumnNotation)col, row)).empty()) {
                                whiteMoves++;
                            }
                            break;
                        case NERO:
                            if (!simulate_damina(NERO,Coords((ColumnNotation)col, row)).empty()) {
                                blackMoves++;
                            }
                            break;
                    }
                    break;
                case DAMONE:
                    switch (board.matrix[row][col].piece.color) {
                        case BIANCO:
                            if (!simulate_damona(Coords((ColumnNotation)col, row)).empty()) {
                                whiteMoves++;
                            }
                        break;
                        case NERO:
                            if (!simulate_damona(Coords((ColumnNotation)col, row)).empty()) {
                                blackMoves++;
                            }
                            break;
                    }
            }
        }
    }

    if (count_pieces(BIANCO) > 0 && whiteMoves == 0){
        return STALEMATE;
    } else if (count_pieces(NERO) > 0 && blackMoves == 0){
        return STALEMATE;
    }  else if (count_pieces(BIANCO) == 0
        && count_pieces(NERO) > 0) {
        return BLACK_WIN;
    } else if (count_pieces(NERO) == 0
        && count_pieces(BIANCO) > 0) {
        return WHITE_WIN;
    } else if (blackMoves == 0 && whiteMoves == 0) {
        return STALEMATE;
    }
    return GAME_NOT_OVER;
}

void GameEngine::execute_command(MoveReturn command) {
    switch (command) {
        case HELP_PAGE:
            RenderV2::help_page();
            break;
        case SUMMARY:
            RenderV2::end_screen(count_pieces(BIANCO), count_pieces(NERO), whitePlayer, blackPlayer, GAME_NOT_OVER, start);
            break;
        default:
            break;
    }
}
