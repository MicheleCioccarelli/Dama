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

std::string GameEngine::get_player_name(PlayerColor color) const{
    if (color == BIANCO) {
        return whitePlayer.name;
    } else {
        return blackPlayer.name;
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

    return board.matrix[tempCoords.row][tempCoords.column].m_piece.m_color;
}

PlayerColor GameEngine::deduce_color_matrix_notation(const Move &move) {
    if (move.is_empty()) {
        return TRASPARENTE;
    }
    Coords tempCoords = move.startingCoord;

    return board.matrix[tempCoords.row][tempCoords.column].m_piece.m_color;
}

void GameEngine::precise_promote(const Move &move) {
    // Assumes matrix-notation
    if (move.endingCoord.row == ROWS - 1 && move.playerColor == BIANCO) {
        board.matrix[move.startingCoord.row][move.startingCoord.column].m_piece.m_type = DAMONE;
    } else if (move.endingCoord.row == 0 && move.playerColor == NERO) {
        board.matrix[move.startingCoord.row][move.startingCoord.column].m_piece.m_type = DAMONE;
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
    if (move.m_wasPromotion) {
        board.matrix[move.startingCoord.row][move.startingCoord.column].m_piece.m_type = DAMA;
        board.matrix[move.endingCoord.row][move.endingCoord.column].m_piece.m_type = DAMA;
    }
    if (move.moveType == EAT) {
        // Swap starting piece and ending piece
        board.matrix[move.startingCoord.row][move.startingCoord.column].m_piece =
                board.matrix[move.endingCoord.row][move.endingCoord.column].m_piece;
        board.matrix[move.endingCoord.row][move.endingCoord.column].m_piece = Piece(TRASPARENTE, VUOTA);

        // Put eaten pieces back on the board
        for (int i = 0; i < move.eatenPieces.size(); i++) {
            board.matrix[move.eatenCoords[i].row][move.eatenCoords[i].column].m_piece = move.eatenPieces[i];
        }
    } else {
        // Swap startingCoords and endingCoords
        Move tempMove = Move(move.endingCoord, move.startingCoord, move.playerColor, MOVE);
        board.execute_move(tempMove);
        // If the piece was promoted, demote it
        if (move.endingCoord.row == ROWS - 1 && deduce_color_matrix_notation(move) == BIANCO) {
            board.matrix[move.endingCoord.row][move.endingCoord.column].m_piece.m_type = DAMA;
        } else if (move.endingCoord.row == 0 && deduce_color_matrix_notation(move) == NERO) {
            board.matrix[move.endingCoord.row][move.endingCoord.column].m_piece.m_type = DAMA;
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
    if (startingSquare.m_piece.m_type == VUOTA) {
        return EMPTY_START;
    }
    if (startingSquare.m_color == BIANCA || endingSquare.m_color == BIANCA) {
        return WHITE_SQUARE;
    }
    // COLORATA is used for debugging purposes and will not show up in a regular game
    if (endingSquare.m_piece.m_type != VUOTA && endingSquare.m_piece.m_type != COLORATA) {
        return POPULATED;
    }

    int verticalDistance = move.startingCoord.row - move.endingCoord.row;
    int horizontalDistance = move.startingCoord.column - move.endingCoord.column;

    // Check if you are moving by one square
    if (verticalDistance == 1 || verticalDistance == -1) {
        if (horizontalDistance == 1 || horizontalDistance == -1) {
            // Check if a damina is moving forwards
            if (startingSquare.m_piece.m_type == DAMA && startingSquare.m_piece.m_color == BIANCO && verticalDistance == 1 ||
                startingSquare.m_piece.m_type == DAMA && startingSquare.m_piece.m_color == NERO && verticalDistance == -1) {
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
    // Assumes matrix-notation
    Square startingSquare = board.matrix[startingCoords.row][startingCoords.column];
    Square endingSquare = board.matrix[endingCoords.row][endingCoords.column];
    // Check square-specific details
    if (endingSquare.m_piece.m_type == VUOTA) {
        return EMPTY_TARGET;
    } else if (startingSquare.m_piece.m_type == VUOTA && dirt == false) {
        return EMPTY_START;
    }
    if (startingSquare.m_color == BIANCA || endingSquare.m_color == BIANCA) {
        return WHITE_SQUARE;
    }
    // Check pieces compatibility
    if (startingSquare.m_piece.m_type == DAMA && endingSquare.m_piece.m_type == DAMONE) {
        return TOO_BIG;
    } else if (startingSquare.m_piece.m_color == endingSquare.m_piece.m_color) {
        return FRIENDLY_FIRE;
    }

    // COLORATA is used for debugging purposes and will not show up in a regular game
    if (endingSquare.m_piece.m_type != VUOTA && endingSquare.m_piece.m_type != COLORATA) {
        return POPULATED;
    }

    int verticalDistance = startingCoords.row - endingCoords.row;
    int horizontalDistance = startingCoords.column - endingCoords.column;
    // Check if you are moving by one square
    if (verticalDistance == 1 || verticalDistance == -1) {
        if (horizontalDistance == 1 || horizontalDistance == -1) {
            // Check if a damina is moving forwards
            if (startingSquare.m_piece.m_type == DAMA && startingSquare.m_piece.m_color == BIANCO && verticalDistance == 1 ||
                startingSquare.m_piece.m_type == DAMA && startingSquare.m_piece.m_color == NERO && verticalDistance == -1) {
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
    if (board.matrix[forwardSquare.row][forwardSquare.column].m_piece.m_type != VUOTA) {
        return POPULATED;
    }
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
    if (tempMove.moveType != EAT) {
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
            if (whitePlayer.moves[whitePlayer.moves.size() - 1].moveType == EAT) {
                return WRONG_LAST_MOVE;
            }
            playerWhoPerformedTheBlowRequest = NERO;
            break;
        case NERO:
            if (blackPlayer.moves[blackPlayer.moves.size() - 1].moveType == EAT) {
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
                if (board.matrix[row][col].m_piece.m_color == BIANCO)
                    returnValue++;
            } else if (pColor == NERO) {
                if (board.matrix[row][col].m_piece.m_color == NERO)
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

    switch (move.moveType) {
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
        if (!tempMove.blownCoord.is_uninitialized()) {
            board.blow_up((Move&) tempMove);
        }
        board.execute_move((Move&) tempMove);
        add_move(tempMove);
    } else {
        UI::log_error(status);
    }
    return status;
}

// ==== GOOD STUFF INCOMING ====

std::vector<Move> GameEngine::simulate_piece(Coords coords) {
    std::vector<Move> moves_found;
    // Put all the EAT type moves the piece can make in the vector
    simulate_eat_piece(moves_found, coords);

    std::vector<Move> temp_moves = simulate_move_piece(coords);

    // Append the MOVE type moves at the end
    moves_found.insert( moves_found.end(), temp_moves.begin(), temp_moves.end());
    return moves_found;
}

void GameEngine::simulate_eat_piece(std::vector<Move>& movesFound, Coords startingCoords, int index) {
    // Tells you if you are trying to eat something your move already ate
    bool alreadyEaten = false;
    // The first time you find a new piece to eat append it to the current move, next time make a new one
    bool alreadyBranched = false;
    // The piece you intend to eat
    Coords eatenCoord;
     // QUESTIONABLE
     if (index == -1) {
        // This is the first call, initialize the vector
        movesFound.emplace_back(Move(startingCoords, EAT));
        index++;
    }
    // Back up the current move
    Move backup = movesFound[index];
    // The for loops allow the function to see in all 4 directions
    for (int verticalOffset = 1; verticalOffset >= -1; verticalOffset -= 2) {
        for (int horizontalOffset = 1; horizontalOffset >= -1; horizontalOffset -= 2) {

            // One of the four squares surrounding the starting damina
            eatenCoord = Coords((ColumnNotation)(startingCoords.column + horizontalOffset), startingCoords.row + verticalOffset);

            // See if you run into a square you have already eaten
            for (Coords i : movesFound[index].eatenCoords) {
                if (eatenCoord == i) {
                    alreadyEaten = true;
                }
            }
            Move tempMove = backup;
            if (!alreadyEaten) {
                tempMove.push_coords(eatenCoord);
                if (recursive_check_eat(tempMove) == ALL_GOOD) {
                    // You found a piece you can eat
                    if (!alreadyBranched) {
                        alreadyBranched = true;
                        movesFound[index].push_coords(eatenCoord);
                    } else {
                        // make a new branch
                        movesFound.push_back(tempMove);
                        index++;
                    }
                    // Investigate the new branch
                    simulate_eat_piece(movesFound, calculate_forward(movesFound[index]), index);
                }
            }
        }
    }
    if (movesFound[index].eatenCoords.empty()) {
        // If you found nothing delete the move
        movesFound.erase(movesFound.end() - 1);
    }
}

std::vector<Move> GameEngine::simulate_move_piece(Coords& coords) {
    std::vector<Move> movesFound;
    Coords endingCoords;
    Move tempMove;

    // See all 4 directions
    for (int verticalOffset = 1; verticalOffset >= -1; verticalOffset -= 2) {
        for (int horizontalOffset = 1; horizontalOffset >= -1; horizontalOffset -= 2) {
            // The square the starting damina would move to
            endingCoords = Coords((ColumnNotation)(coords.column + horizontalOffset), coords.row + verticalOffset);

            tempMove = Move(coords, endingCoords, MOVE);

            if (check_move(tempMove) == ALL_GOOD) {
                movesFound.push_back(tempMove);
            }
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
            switch (board.matrix[i][j].m_piece.m_color) {
                   case BIANCO:
                       // simulate_damina returns the vector of oves that damina can perform
                       whiteMoves += simulate_piece(Coords((ColumnNotation)j, i)).size();
                       break;
                   case NERO:
                       blackMoves += simulate_piece(Coords((ColumnNotation)j, i)).size();
                       break;
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

#pragma clang diagnostic pop