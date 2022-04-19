#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
#include "engine.h"
#include "../ui/ui.h"

// ====== GAME ENGINE ======
GameEngine::GameEngine(GameStyle gameStyle)
        : m_render(), m_board() {
    m_start = time(nullptr);
    switch (gameStyle) {
        case STANDARD:
            m_board.standard_game_initialization();
            break;
        case DAMONI:
            m_board.damone_game_initialization();
            break;
        case COLORED:
            m_board.colored_game_initialization();
            break;
        case EMPTY:
            m_board.empty_game_initialization();
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
    Coords tempCoords = move.m_startingCoord.convert_coords();

    return m_board.matrix[tempCoords.row][tempCoords.column].m_piece.m_color;
}

PlayerColor GameEngine::deduce_color_matrix_notation(const Move &move) {
    if (move.is_empty()) {
        return TRASPARENTE;
    }
    Coords tempCoords = move.m_startingCoord;

    return m_board.matrix[tempCoords.row][tempCoords.column].m_piece.m_color;
}

void GameEngine::precise_promote(const Move &move) {
    // Assumes matrix-notation
    if (move.m_endingCoord.row == ROWS - 1 && move.m_playerColor == BIANCO) {
        m_board.matrix[move.m_startingCoord.row][move.m_startingCoord.column].m_piece.m_type = DAMONE;
    } else if (move.m_endingCoord.row == 0 && move.m_playerColor == NERO) {
        m_board.matrix[move.m_startingCoord.row][move.m_startingCoord.column].m_piece.m_type = DAMONE;
    }
}

void GameEngine::add_move(const Move &move) {
    if (move.m_playerColor == BIANCO) {
        m_whitePlayer.add_move(move);
    } else {
        m_blackPlayer.add_move(move);
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
    Coords tempForward = calculate_forward(move.m_startingCoord, move.m_eatenCoords[0]);
    for (int i = 1; i < move.m_eatenCoords.size(); i++) {
        tempForward = calculate_forward(tempForward, move.m_eatenCoords[i]);
    }
    return tempForward;
}

bool GameEngine::time_travel(PlayerColor startingPlayer, int depth, bool goingBackwards) {
    Player opponent;
    int whiteIndex = m_whitePlayer.m_moves.size() - 1;
    int blackIndex = m_blackPlayer.m_moves.size() - 1;

    if (startingPlayer == BIANCO) {
        opponent = m_blackPlayer;
        // depth/2 + 1 is the number of enemy moves that will be manipulated, see if it is possible
        if (m_blackPlayer.m_moves.size() - ((depth / 2) + depth % 2) < 0 || m_whitePlayer.m_moves.size() - (depth / 2) < 0) {
            return false;
        }
    } else {
        if (m_whitePlayer.m_moves.size() - ((depth / 2) + depth % 2) < 0 || m_blackPlayer.m_moves.size() - (depth / 2) < 0) {
            return false;
        }
        opponent  = m_whitePlayer;
    }
    // Works backwards
    if (goingBackwards) {
        // Undo the move recursively
        return playBack(opponent.m_color, depth, whiteIndex, blackIndex);
    } else {
        // You want to go forward in time
        Player currentPlayer;
        if (startingPlayer == BIANCO) {
            // The +1 is because you want to redo the newest move as well
            whiteIndex = (whiteIndex - depth / 2) + 1;
            blackIndex = (blackIndex - ((depth / 2) + depth % 2)) + 1;

            while (depth > 0) {
                precise_promote(m_blackPlayer.m_moves[blackIndex]);
                m_board.execute_move(m_blackPlayer.m_moves[blackIndex++]);
                depth--;
                if (depth <= 0)
                    break;
                precise_promote(m_whitePlayer.m_moves[whiteIndex]);
                m_board.execute_move(m_whitePlayer.m_moves[whiteIndex++]);
                depth--;
            }
            return true;
        } else {
            blackIndex = (blackIndex - depth / 2) + 1;
            whiteIndex = (whiteIndex - ((depth / 2) + depth % 2)) + 1;

            while (depth > 0) {
                precise_promote(m_whitePlayer.m_moves[whiteIndex]);
                m_board.execute_move(m_whitePlayer.m_moves[whiteIndex++]);
                depth--;
                if (depth <= 0)
                    break;
                precise_promote(m_blackPlayer.m_moves[blackIndex]);
                m_board.execute_move(m_blackPlayer.m_moves[blackIndex++]);
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
        undo_move(m_whitePlayer.m_moves[whiteIndex--]);
        return playBack(NERO, depth - 1,  whiteIndex, blackIndex);
    } else {
        undo_move(m_blackPlayer.m_moves[blackIndex--]);
        return playBack(BIANCO, depth - 1,  whiteIndex, blackIndex);
    }
}

void GameEngine::undo_move(const Move &move) {
    // Assumes matrix-notation
    if (move.m_wasPromotion) {
        m_board.matrix[move.m_startingCoord.row][move.m_startingCoord.column].m_piece.m_type = DAMA;
        m_board.matrix[move.m_endingCoord.row][move.m_endingCoord.column].m_piece.m_type = DAMA;
    }
    if (move.m_type == EAT) {
        // Swap starting piece and ending piece
        m_board.matrix[move.m_startingCoord.row][move.m_startingCoord.column].m_piece =
                m_board.matrix[move.m_endingCoord.row][move.m_endingCoord.column].m_piece;
        m_board.matrix[move.m_endingCoord.row][move.m_endingCoord.column].m_piece = Piece(TRASPARENTE, VUOTA);

        // Put eaten pieces back on the board
        for (int i = 0; i < move.m_eatenPieces.size(); i++) {
            m_board.matrix[move.m_eatenCoords[i].row][move.m_eatenCoords[i].column].m_piece = move.m_eatenPieces[i];
        }
    } else {
        // Swap startingCoords and endingCoords
        Move tempMove = Move(move.m_endingCoord, move.m_startingCoord, move.m_playerColor, MOVE);
        m_board.execute_move(tempMove);
        // If the piece was promoted, demote it
        if (move.m_endingCoord.row == ROWS - 1 && deduce_color_matrix_notation(move) == BIANCO) {
            m_board.matrix[move.m_endingCoord.row][move.m_endingCoord.column].m_piece.m_type = DAMA;
        } else if (move.m_endingCoord.row == 0 && deduce_color_matrix_notation(move) == NERO) {
            m_board.matrix[move.m_endingCoord.row][move.m_endingCoord.column].m_piece.m_type = DAMA;
        }
    }
}

MoveIssue GameEngine::check_move(Move &move) {
    if (!is_in_bounds(move.m_startingCoord) || !is_in_bounds(move.m_endingCoord)) {
        return OUT_OF_BOUNDS;
    }

    Square startingSquare = m_board.matrix[move.m_startingCoord.row][move.m_startingCoord.column];
    Square endingSquare = m_board.matrix[move.m_endingCoord.row][move.m_endingCoord.column];

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

    int verticalDistance = move.m_startingCoord.row - move.m_endingCoord.row;
    int horizontalDistance = move.m_startingCoord.column - move.m_endingCoord.column;

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
    Square startingSquare = m_board.matrix[startingCoords.row][startingCoords.column];
    Square endingSquare = m_board.matrix[endingCoords.row][endingCoords.column];
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
        startingCoords = move.m_startingCoord;
        dirt = false;
    } else if (index == move.m_eatenCoords.size()) {
        // You are at the end of the move.coords vector, everything went fine (base case)
        return ALL_GOOD;
    }
    // This is where the dama would go if it ate endingcoords
    Coords forwardSquare = calculate_forward(startingCoords, move.m_eatenCoords[index]);
    if (m_board.matrix[forwardSquare.row][forwardSquare.column].m_piece.m_type != VUOTA) {
        return POPULATED;
    }
    if (is_in_bounds(forwardSquare)) {
        // Check if the move doesn't break the rules of the game
        MoveIssue result = inspect_dama(startingCoords, move.m_eatenCoords[index], dirt);
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
    if (tempMove.m_type != EAT) {
        return WRONG_TYPE;
    }
    // Assumes in-bounds matrix-notation input
    PlayerColor opponent = deduce_color_matrix_notation(tempMove);
    if (tempMove.m_playerColor == opponent) {
        return WRONG_COLOR;
    }
    // Dirt
    PlayerColor playerWhoPerformedTheBlowRequest;
    switch (opponent) {
        case BIANCO:
            if (m_whitePlayer.m_moves[m_whitePlayer.m_moves.size() - 1].m_type == EAT) {
                return WRONG_LAST_MOVE;
            }
            playerWhoPerformedTheBlowRequest = NERO;
            break;
        case NERO:
            if (m_blackPlayer.m_moves[m_blackPlayer.m_moves.size() - 1].m_type == EAT) {
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
                if (m_board.matrix[row][col].m_piece.m_color == BIANCO)
                    returnValue++;
            } else if (pColor == NERO) {
                if (m_board.matrix[row][col].m_piece.m_color == NERO)
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

    switch (move.m_type) {
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
        if (!move.m_blownCoord.is_uninitialized()) {
            move.m_blownCoord = move.m_blownCoord.convert_coords();
            m_board.blow_up((Move&) tempMove);
        }
        m_board.execute_move((Move&) tempMove);
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
            for (Coords i : movesFound[index].m_eatenCoords) {
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
    if (movesFound[index].m_eatenCoords.empty()) {
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
            switch (m_board.matrix[i][j].m_piece.m_color) {
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

bool GameEngine::execute_command(MoveData command) const {
    char input;
    // Returns true if the game should be ended
    switch (command) {
        case HELP_PAGE:
            RenderV2::help_page();
            return false;
        case SUMMARY:
            RenderV2::end_screen(count_pieces(BIANCO), count_pieces(NERO), m_whitePlayer, m_blackPlayer, GAME_NOT_OVER, m_start);
            return false;
        case WHITE_RESIGN:
            RenderV2::end_screen(count_pieces(BIANCO), count_pieces(NERO),
                                 m_whitePlayer, m_blackPlayer, WHITE_RESIGNED, m_start);
            return true;
        case BLACK_RESIGN:
            RenderV2::end_screen(count_pieces(BIANCO), count_pieces(NERO),
                                 m_whitePlayer, m_blackPlayer, BLACK_RESIGNED, m_start);
            return true;
        case W_DRAW_OFFER:
            std::cout << m_blackPlayer.m_name << " accetta il pareggio? [" << ACCEPT_DRAW << "s" << RESET << "/" << REFUSE_DRAW << "n"
            << RESET << "]\n";
            std::cin >> input;
            if (input == 's') {
                RenderV2::end_screen(count_pieces(BIANCO), count_pieces(NERO),
                                     m_whitePlayer, m_blackPlayer, DRAW, m_start);
                return true;
            } else {
                std::cout << "Niente pareggio\n" << std::endl;
                std::cin.ignore(1);
                return false;
            }
        case B_DRAW_OFFER:
            std::cout << m_blackPlayer.m_name << " accetta il pareggio? [" << ACCEPT_DRAW << "s" << RESET << "/" << REFUSE_DRAW << "n"
                      << RESET << "]\n";
            std::cin >> input;
            if (input == 's') {
                RenderV2::end_screen(count_pieces(BIANCO), count_pieces(NERO),
                                     m_whitePlayer, m_blackPlayer, DRAW, m_start);
                return true;
            } else {
                std::cout << "Niente pareggio" << std::endl;
                std::cin.ignore(1);
                return false;
            }
        case QUIT:
            return true;
    }
    return false;
}

#pragma clang diagnostic pop