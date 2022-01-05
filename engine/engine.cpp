#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
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

PlayerColor GameEngine::deduce_color_human_notation(Move &move) {

    if (move.is_empty()) {
        return TRASPARENTE;
    }
    Coords tempCoords = move.startingCoord.convert_coords();

    return board.matrix[tempCoords.row][tempCoords.column].piece.color;
}

PlayerColor GameEngine::deduce_color_matrix_notation(const Move &move) {
    if (move.is_empty()) {
        return TRASPARENTE;
    }
    Coords tempCoords = move.startingCoord;

    return board.matrix[tempCoords.row][tempCoords.column].piece.color;
}

void GameEngine::precise_promote(const Move &move) {
    // Assumes matrix-notation
    if (move.endingCoord.row == ROWS - 1 && move.playerColor == BIANCO) {
        board.matrix[move.startingCoord.row][move.startingCoord.column].piece.type = DAMONE;
    } else if (move.endingCoord.row == 0 && move.playerColor == NERO) {
        board.matrix[move.startingCoord.row][move.startingCoord.column].piece.type = DAMONE;
    }
}

void GameEngine::add_move(const Move &move) {
    if (move.playerColor == BIANCO) {
        whitePlayer.add_move(move);
    } else {
        blackPlayer.add_move(move);
    }
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
    Coords tempForward = calculate_forward(move.startingCoord, move.eatenCoords[0]);
    for (int i = 1; i < move.eatenCoords.size(); i++) {
        tempForward = calculate_forward(tempForward, move.eatenCoords[i]);
    }
    return tempForward;
}

bool GameEngine::time_travel(PlayerColor startingPlayer, int depth, bool goingBackwards) {
    Player opponent;
    int whiteIndex = whitePlayer.moves.size() - 1;
    int blackIndex = blackPlayer.moves.size() - 1;

    if (startingPlayer == BIANCO) {
        opponent = blackPlayer;
        // depth/2 + 1 is the number of enemy moves that will be manipulated, see if it is possible
        if (blackPlayer.moves.size() - ((depth / 2) + depth % 2) < 0 || whitePlayer.moves.size() - (depth / 2) < 0) {
            return false;
        }
    } else {
        if (whitePlayer.moves.size() - ((depth / 2) + depth % 2) < 0 || blackPlayer.moves.size() - (depth / 2) < 0) {
            return false;
        }
        opponent  = whitePlayer;
    }
    // Works backwards
    if (goingBackwards) {
        // Undo the move recursively
        return playBack(opponent.color, depth, whiteIndex, blackIndex);
    } else {
        // You want to go forward in time
        Player currentPlayer;
        if (startingPlayer == BIANCO) {
            // The +1 is because you want to redo the newest move as well
            whiteIndex = (whiteIndex - depth / 2) + 1;
            blackIndex = (blackIndex - ((depth / 2) + depth % 2)) + 1;

            while (depth > 0) {
                precise_promote(blackPlayer.moves[blackIndex]);
                board.execute_move(blackPlayer.moves[blackIndex++]);
                depth--;
                if (depth <= 0)
                    break;
                precise_promote(whitePlayer.moves[whiteIndex]);
                board.execute_move(whitePlayer.moves[whiteIndex++]);
                depth--;
            }
            return true;
        } else {
            blackIndex = (blackIndex - depth / 2) + 1;
            whiteIndex = (whiteIndex - ((depth / 2) + depth % 2)) + 1;

            while (depth > 0) {
                precise_promote(whitePlayer.moves[whiteIndex]);
                board.execute_move(whitePlayer.moves[whiteIndex++]);
                depth--;
                if (depth <= 0)
                    break;
                precise_promote(blackPlayer.moves[blackIndex]);
                board.execute_move(blackPlayer.moves[blackIndex++]);
                depth--;
            }
            return true;
        }
    }
}

bool GameEngine::playBack(PlayerColor currentPlayer, int depth, int whiteIndex, int blackIndex) {
    // Base case
    if (depth <= 0) {
        return true;
    }
    // The previous iteration did a black player if this is not the first call
    if (currentPlayer == BIANCO) {
        undo_move(whitePlayer.moves[whiteIndex--]);
        return playBack(NERO, depth - 1,  whiteIndex, blackIndex);
    } else {
        undo_move(blackPlayer.moves[blackIndex--]);
        return playBack(BIANCO, depth - 1,  whiteIndex, blackIndex);
    }
}

void GameEngine::undo_move(const Move &move) {
    // Assumes matrix-notation
    if (move.wasPromotion) {
        board.matrix[move.startingCoord.row][move.startingCoord.column].piece.type = DAMA;
        board.matrix[move.endingCoord.row][move.endingCoord.column].piece.type = DAMA;
    }
    if (move.type == EAT) {
        // Swap starting piece and ending piece
        board.matrix[move.startingCoord.row][move.startingCoord.column].piece =
                board.matrix[move.endingCoord.row][move.endingCoord.column].piece;
        board.matrix[move.endingCoord.row][move.endingCoord.column].piece = Piece(TRASPARENTE, VUOTA);

        // Put eaten pieces back on the board
        for (int i = 0; i < move.eatenPieces.size(); i++) {
            board.matrix[move.eatenCoords[i].row][move.eatenCoords[i].column].piece = move.eatenPieces[i];
        }
    } else {
        // Swap startingCoords and endingCoords
        Move tempMove = Move(move.endingCoord, move.startingCoord, move.playerColor, MOVE);
        board.execute_move(tempMove);
        // If the piece was promoted, demote it
        if (move.endingCoord.row == ROWS - 1 && deduce_color_matrix_notation(move) == BIANCO) {
            board.matrix[move.endingCoord.row][move.endingCoord.column].piece.type = DAMA;
        } else if (move.endingCoord.row == 0 && deduce_color_matrix_notation(move) == NERO) {
            board.matrix[move.endingCoord.row][move.endingCoord.column].piece.type = DAMA;
        }
    }
}

MoveIssue GameEngine::check_move(Move &move) {
    if (!is_in_bounds(move.startingCoord) || !is_in_bounds(move.endingCoord)) {
        return OUT_OF_BOUNDS;
    }

    Square startingSquare = board.matrix[move.startingCoord.row][move.startingCoord.column];
    Square endingSquare = board.matrix[move.endingCoord.row][move.endingCoord.column];

    // Check square-specific details
    if (startingSquare.piece.type == VUOTA) {
        return EMPTY_START;
    }
    if (startingSquare.color == BIANCA || endingSquare.color == BIANCA) {
        return WHITE_SQUARE;
    }
    // COLORATA is used for debugging purposes and will not show up in a regular game
    if (endingSquare.piece.type != VUOTA && endingSquare.piece.type != COLORATA) {
        return POPULATED;
    }

    int verticalDistance = move.startingCoord.row - move.endingCoord.row;
    int horizontalDistance = move.startingCoord.column - move.endingCoord.column;

    // Check if you are moving by one square
    if (verticalDistance == 1 || verticalDistance == -1) {
        if (horizontalDistance == 1 || horizontalDistance == -1) {
            // Check if a damina is moving forwards
            if (startingSquare.piece.type == DAMA && startingSquare.piece.color == BIANCO && verticalDistance == 1 ||
                startingSquare.piece.type == DAMA && startingSquare.piece.color == NERO && verticalDistance == -1) {
                return BEHIND;
            }
            return ALL_GOOD;
        } else {
            return TOO_FAR;
        }
    } else {
        return TOO_FAR;
    }
}

MoveIssue GameEngine::inspect_dama(Coords startingCoords, Coords endingCoords, bool dirt) {
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
            return ALL_GOOD;
        } else {
            return TOO_FAR;
        }
    } else {
        return TOO_FAR;
    }
}

MoveIssue GameEngine::recursive_check_eat(Move move, Coords startingCoords, int index) {
    // Assumes a move with valid syntax and matrix notation
    /* When you check for multiple eatings the position you start from is actually empty because
     * this function doesn't move pieces around, dirt tells inspect_dama() to ignore the emptiness
     */
    bool dirt = true;
    if (index == 0) {
        // This is the first call as index defaults to 0
        startingCoords = move.startingCoord;
        dirt = false;
    } else if (index == move.eatenCoords.size()) {
        // You are at the end of the move.coords vector, everything went fine (base case)
        return ALL_GOOD;
    }
    // This is where the dama would go if it ate endingcoords
    Coords forwardSquare = calculate_forward(startingCoords, move.eatenCoords[index]);
    if (is_in_bounds(forwardSquare)) {
        // Check if the move doesn't break the rules of the game
        MoveIssue result = inspect_dama(startingCoords, move.eatenCoords[index], dirt);
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

MoveIssue GameEngine::check_blow(Move& move) {
    // Assumes human-notation
    Move tempMove = move;
    tempMove.convert_all();
    if (tempMove.is_misinput()) {
        return MISINPUT;
    }
    if (tempMove.type != EAT) {
        return WRONG_TYPE;
    }
    // Assumes in-bounds matrix-notation input
    PlayerColor opponent = deduce_color_matrix_notation(tempMove);
    if (tempMove.playerColor == opponent) {
        return WRONG_COLOR;
    }
    // Dirt
    PlayerColor playerWhoPerformedTheBlowRequest;
    switch (opponent) {
        case BIANCO:
            if (whitePlayer.moves[whitePlayer.moves.size() - 1].type == EAT) {
                return WRONG_LAST_MOVE;
            }
            playerWhoPerformedTheBlowRequest = NERO;
            break;
        case NERO:
            if (blackPlayer.moves[blackPlayer.moves.size() - 1].type == EAT) {
                return WRONG_LAST_MOVE;
            }
            playerWhoPerformedTheBlowRequest = BIANCO;
        default:
            // Shouldn't happen
            return MISINPUT;
    }
    // Go back to when before your opponent made their last move
    if (!time_travel(playerWhoPerformedTheBlowRequest, 1, true)) {
        // If this is false the opponent is yet to make a move
        return NOT_ENOUGH_MOVES;
    }
    // Check if your opponent could have made their move
    if (recursive_check_eat(tempMove) == ALL_GOOD) {
        // Re-bring the board to before check_blow was called
        time_travel(playerWhoPerformedTheBlowRequest, 1, false);
        return BLOWABLE;
    } else {
        time_travel(playerWhoPerformedTheBlowRequest, 1, false);
        return ROCK_SOLID;
    }
}

int GameEngine::count_pieces(PlayerColor pColor) const {
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

MoveIssue GameEngine::submit(Move& move) {
    // Assumes human-notation
    // If this is called the move has no syntactical errors
    MoveIssue status;

    // Create a new move with it's coords converted to matrix notation, this makes it readable by check functions
    Move tempMove = move;
    tempMove.convert_all();

    switch (move.type) {
        case MOVE:
            status = check_move(tempMove);
            break;
        case EAT:
            status = recursive_check_eat(tempMove);
            break;
        default:
            status = UNDEFINED;
    }
    if (status == ALL_GOOD) {
        // Dispatch the move
        if (!move.blownCoord.is_uninitialized()) {
            move.blownCoord = move.blownCoord.convert_coords();
            board.blow_up((Move&) tempMove);
        }
        board.execute_move((Move&) tempMove);
        add_move(tempMove);
    } else {
        UI::log_error(status);
    }
    return status;
}

std::vector<Move> GameEngine::branch_damina(Coords startingCoords, PlayerColor color, int verticalOffset, int horizontalOffset) {
    std::vector<Move> movesFound;
    // The square the starting damina would move to
    Coords endingCoords = Coords((ColumnNotation)(startingCoords.column + horizontalOffset),
                                 startingCoords.row + verticalOffset);
    Move move = Move(startingCoords, endingCoords, color);
    MoveIssue result = check_move(move);
    switch (result) {
        case ALL_GOOD:
            // A move-type move can be executed
            move.type = MOVE;
            movesFound.push_back(move);
            break;
        case POPULATED:
            // There is a dama in the ending square
        // MISSING STUFF
            break;
    }
    return movesFound;
}

std::vector<Move> GameEngine::simulate_damina(PlayerColor daminaColor, Coords coords) {
    // Input coords are assumed to be in matrix notation
    std::vector<Move> movesFound;
    std::vector<Move> tempMoves;

    /*
     * For the damine we check what moves are in front of them
     * (add 1 to the row for white and leave black because check_eat already subtracts 1)
     * The damone are also checked behind
     * */


    switch (daminaColor) {
        case BIANCO:
            // Bounds check
            if ((int)coords.column - 1 < 0) {
                break;
            }
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
            if (coords.row - 1 < 0) {
                break;
            }
            tempMoves = branch_damina(coords, NERO, -1, 1);
            if (!tempMoves.empty()) {
                for (const auto &i: tempMoves) {
                    movesFound.push_back(i);
                }
            }
            if (coords.row - 1 < 0) {
                break;
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

GameState GameEngine::game_over() {
    // The game was not ended by a command
    int whiteMoves = 0;
    int blackMoves = 0;

    // Check how many moves each piece can make
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            switch(board.matrix[i][j].piece.type) {
                case DAMA:
                    switch (board.matrix[i][j].piece.color) {
                        case BIANCO:
                            // simulate_damina returns the vector of oves that damina can perform
                            whiteMoves += simulate_damina(BIANCO, Coords((ColumnNotation)j, i)).size();
                            break;
                        case NERO:
                            blackMoves += simulate_damina(NERO, Coords((ColumnNotation) j, i)).size();
                            break;
                        default:
                            break;
                    }
                    break;
                case DAMONE:
                    switch (board.matrix[i][j].piece.color) {
                        case BIANCO:
                            // simulate_damina returns the vector of oves that damina can perform
                            whiteMoves += simulate_damona(Coords((ColumnNotation) j, i)).size();
                            break;
                        case NERO:
                            blackMoves += simulate_damona( Coords((ColumnNotation) j, i)).size();
                            break;
                        default:
                            break;
                    }
                default:
                    break;
            }
        }
    }
    if (whiteMoves > 0 && blackMoves > 0) {
        return GAME_NOT_OVER;
    } else if (blackMoves == 0 && whiteMoves > 0) {
        return WHITE_WIN;
    } else if (whiteMoves == 0 && blackMoves > 0) {
        return BLACK_WIN;
    } else if (whiteMoves == 0 && blackMoves == 0) {
        return STALEMATE;
    }
    // Should not reach this point
    return GAME_NOT_OVER;
}

bool GameEngine::execute_command(MoveData command) const {
    char input;
    // Returns true if the game should be ended
    switch (command) {
        case HELP_PAGE:
            RenderV2::help_page();
            return false;
        case SUMMARY:
            RenderV2::end_screen(count_pieces(BIANCO), count_pieces(NERO), whitePlayer, blackPlayer, GAME_NOT_OVER, start);
            return false;
        case WHITE_RESIGN:
            RenderV2::end_screen(count_pieces(BIANCO), count_pieces(NERO),
                                 whitePlayer, blackPlayer, WHITE_RESIGNED, start);
            return true;
        case BLACK_RESIGN:
            RenderV2::end_screen(count_pieces(BIANCO), count_pieces(NERO),
                                 whitePlayer, blackPlayer, BLACK_RESIGNED, start);
            return true;
        case W_DRAW_OFFER:
            std::cout << blackPlayer.name << " accetta il pareggio? [s/n]\n";
            std::cin >> input;
            if (input == 's') {
                RenderV2::end_screen(count_pieces(BIANCO), count_pieces(NERO),
                                     whitePlayer, blackPlayer, DRAW, start);
                return true;
            } else {
                std::cout << "Niente pareggio\n" << std::endl;
                std::cin.ignore(1);
                return false;
            }
        case B_DRAW_OFFER:
            std::cout << whitePlayer.name << " accetta il pareggio? [s/n]\n";
            std::cin >> input;
            if (input == 's') {
                RenderV2::end_screen(count_pieces(BIANCO), count_pieces(NERO),
                                     whitePlayer, blackPlayer, DRAW, start);
                return true;
            } else {
                std::cout << "Niente pareggio" << std::endl;
                std::cin.ignore(1);
                return false;
            }
    }
    return false;
}

#pragma clang diagnostic pop