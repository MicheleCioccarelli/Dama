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
    if (move.is_empty()) {
        return TRASPARENTE;
    }
    Coords tempCoords = move.startingCoord.convert_coords();

    return board.matrix[tempCoords.row][tempCoords.column].piece.color;
}

void GameEngine::dispatch_move(const Move& move) {
    // Assumes matrix-notation input
    // Add the move to the respective player
    if (move.playerColor == BIANCO) {
        whitePlayer.add_move(move);
    } else if (move.playerColor == NERO) {
        blackPlayer.add_move(move);
    }
    if (move.blownCoord.is_uninitialized()) {
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
                board.execute_move(blackPlayer.moves[whiteIndex++]);
                depth--;
                if (depth <= 0)
                    break;

                board.execute_move(whitePlayer.moves[blackIndex++]);
                depth--;
            }
            return true;
        } else {
            blackIndex = (blackIndex - depth / 2) + 1;
            whiteIndex = (whiteIndex - ((depth / 2) + depth % 2)) + 1;

            while (depth > 0) {
                board.execute_move(whitePlayer.moves[whiteIndex++]);
                depth--;
                if (depth <= 0)
                    break;

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

    // FILL ENDINGCOORD

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
    PlayerColor opponent = deduce_color(tempMove);
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
            return WRONG_COLOR;
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
        dispatch_move(tempMove);
    } else {
        UI::log_error(status);
    }
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
        if (check_move(move) == ALL_GOOD) {
            movesFound.push_back(move);
        } else if (recursive_check_eat(move) == ALL_GOOD){
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

void GameEngine::resign(MoveData command) const {
    int whitePieces = count_pieces(BIANCO);
    int blackPieces = count_pieces(NERO);

    if (command == WHITE_RESIGN) {
        RenderV2::end_screen(whitePieces, blackPieces, whitePlayer, blackPlayer, WHITE_RESIGNED, start);
    } else if (command == BLACK_RESIGN) {
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

void GameEngine::execute_command(MoveData command) const {
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
