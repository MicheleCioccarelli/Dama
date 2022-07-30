#include "apium.h"

Apium::Apium(GameEngine &Engine)
    : engine(Engine) {}

Apium::Apium(GameEngine &Engine, Playstyle Playstyle, PlayerColor ApiumColor)
    : engine(Engine), playstyle(Playstyle), whoIsBeingPlayed(ApiumColor) {}

Apium::Apium(Playstyle PlayStyle, PlayerColor ApiumColor)
    : playstyle(PlayStyle), whoIsBeingPlayed(ApiumColor) {}

void Apium::set_eval(float Eval) {
    Eval = Eval;
}

void Apium::setEngine(const GameEngine &rhs) {
    engine = rhs;
}

void Apium::set_playstyle(const Playstyle Playstyle) {
    playstyle = Playstyle;
}

float Apium::get_eval() const { return gameEval; }

void Apium::update_eval() {
    set_eval(evaluate_current_position());
}

void Apium::sync_engine(const GameEngine &Engine) {
    engine = Engine;
}

float Apium::evaluate_piece(Coords pieceCoords, Piece piece) const {
    // Expects matrix-notation
    float pieceEval = 0;

    if (piece == Piece()) {
        piece = engine.board.matrix[pieceCoords.row][pieceCoords.column].piece;
    }

    // There was no piece in the position
    if (piece.type == VUOTA) {
        return pieceEval;
    }

    /**
     * Ideas for improvement:
     * Reward damone based on how close they are to a piece they can ear, delete rewarding based on
     * normal position for them
     * */

     switch (playstyle) {
        case NEUTRAL:
        /* EVALUATION TABLE FOR NEUTRAL
         * DAMINA -> +2   | Increment based on position: +0.25
         * DAMONA -> +3   | Increment based on position: +0.25
         */
            if (piece.type == DAMA) {
                if (piece.color == BIANCO) {
                    // The base value for a damina is 2
                    pieceEval += 2;

                    // Add .25 based on how close it is to being a damone (1 square closer = +.25)
                    pieceEval += pieceCoords.row * .25;
                } else {
                    // Black damina
                    pieceEval -= 2;

                    // Same as above but inverted
                    pieceEval -= (7 - pieceCoords.row) * .25;
                }
            } else {
                pieceEval += 3;
            }
            break;
         case AGGRESSIVE:
         case DEFENSIVE:
             break;
    }
    return pieceEval;
}

float Apium::evaluate_current_position() const {
    float returnValue = 0;
    // Evaluate all the black pieces
    for (auto& square : engine.blackPiecesSquares)
                returnValue += evaluate_piece(square.coords, square.piece);
    // evaluate all the white pieces
    for (auto& square : engine.whitePiecesSquares)
        returnValue += evaluate_piece(square.coords, square.piece);
    return returnValue;
}

float Apium::evaluate_board_position(std::string &currentBoadPos) const {
    float returnValue = 0;
    std::vector<Square> squares;
    // For some reason the = operator doesn't work
    for (const Square& square : BoardPos::notation_to_squares(currentBoadPos)) {
        squares.push_back(square);
    }

    for (const Square& square : squares) {
        returnValue += evaluate_piece(square.coords, square.piece);
    }
    return returnValue;
}

Move Apium::find_best_move(PlayerColor whoIsPlaying, bool shouldCleanup) {
    // Keeps track of the current best score
    float bestEval = gameEval;
    float newEval = bestEval;
    Move bestMove;
    // The first time the loop runs a move gets assigned to bestMove, so that if all the moves suck the function
    // doesn't return an empty move
    bool firstLoop = true;

    if (whoIsPlaying == BIANCO) {
        // White is trying to maximize the gameEval and moves the white pieces
        for (auto &square: engine.whitePiecesSquares) { // For each white piece on the board
            for (const auto &currentMove: engine.simulate_piece(square.coords)) {
                // For each of the moves the piece could make
                if (firstLoop) {
                    firstLoop = false;
                    bestMove = currentMove;
                }
                engine.submit_matrix_notation(currentMove, whoIsPlaying); // play currentMove

                newEval = evaluate_current_position();
                if (newEval > bestEval) {
                    bestEval = newEval;
                    bestMove = currentMove;
                }
                engine.undo_move(currentMove); // cleanup after you played currentMove
                engine.refresh_piece_vectors();
            }
        }
    } else {
        // Black wants to minimize the score and plays with the black pieces
        for (auto& square : engine.blackPiecesSquares) { // For each black piece on the board
            for (const auto& currentMove : engine.simulate_piece(square.coords)) {
                // For each of the moves the piece could make
                if (firstLoop) {
                    firstLoop = false;
                    bestMove = currentMove;
                }
                engine.submit_matrix_notation(currentMove, whoIsPlaying); // play currentMove

                newEval = evaluate_current_position();
                if (newEval < bestEval) {
                    bestEval = newEval;
                    bestMove = currentMove;
                }
                engine.undo_move(currentMove); // cleanup after you played currentMove
                engine.refresh_piece_vectors();
            }
        }
    }
    return bestMove;
}

// Ahhh shit, here we go again
float Apium::minimax(size_t depth, PlayerColor whoIsPlaying, bool  isFirstTime) {
    if (depth == 0 || engine.game_over() != GAME_NOT_OVER) {
        return evaluate_current_position();
    }
    float eval = 0;
    float bestEval = 0;

    if (whoIsPlaying == BIANCO) {
        float maxEval = -100000;
        for (auto &square: engine.whitePiecesSquares) { // For each white piece on the board
            for (const auto &currentMove: engine.simulate_piece(square.coords)) {
                // Perform the move
                engine.submit_matrix_notation(currentMove, BIANCO);
                engine.refresh_piece_vectors();

                eval = minimax(depth - 1, NERO, false);
                maxEval = std::max(eval, maxEval);

                // If you are at the top of the tree you can pick the best move
                if (isFirstTime) {
                    if (eval > bestEval) {}
                    bestLine = ApiumLine(currentMove, bestEval);
                }

                // Cleanup
                engine.undo_move(currentMove);
                engine.refresh_piece_vectors();
            }
        }
        return maxEval;
    } else {
        float minEval = 100000;
        for (auto &square: engine.blackPiecesSquares) { // For each white piece on the board
            for (const auto &currentMove: engine.simulate_piece(square.coords)) {
                // Perform the move
                engine.submit_matrix_notation(currentMove, NERO);
                engine.refresh_piece_vectors();

                eval = minimax(depth - 1, BIANCO, false);
                minEval = std::min(eval, minEval);

                // If you are at the top of the tree you can pick the best move
                if (isFirstTime) {
                    if (eval < bestEval) {}
                    bestLine = ApiumLine(currentMove, bestEval);
                }

                // Cleanup
                engine.undo_move(currentMove);
                engine.refresh_piece_vectors();
            }
        }
        return minEval;
    }
}

/**
 * Idea for find_best_line()'s redesign:
 * First, you generate a tree of all the possible position that can be reached in a certain number of moves.
 * Once you get to the end you can start deciding which move is best out of the ones available
 * */

ApiumLine Apium::find_best_line(int depth, PlayerColor moveMaker, ApiumLine beingConstructed) {
    switch (engine.game_over()) {
        case GAME_NOT_OVER:
            break;
        case BLACK_WIN:
            return {beingConstructed.get_moves(), -100000};
        case WHITE_WIN:
            return {beingConstructed.get_moves(), +100000};
        default:
            break;
    }
    if (depth == 0) {
        return {beingConstructed.get_moves(), evaluate_current_position()};
    }
    // The moves which can be made at this point of the recursion
    std::vector<ApiumLine> bestLineCandidates;
    float currentEval {};

    if (moveMaker == BIANCO) {
        for (auto &square: engine.whitePiecesSquares) { // For each white piece on the board
            for (const auto &currentMove: engine.simulate_piece(square.coords)) { // For each move a piece can make
                // Make the move
                beingConstructed.push_move(currentMove);
                engine.submit_matrix_notation(currentMove, BIANCO);
                engine.refresh_piece_vectors();

                // Add to the candidates the move that was just checked
                bestLineCandidates.push_back(find_best_line(depth - 1, NERO, beingConstructed));

                // Cleanup
                beingConstructed.pop_move();
                engine.undo_move(currentMove);
                engine.refresh_piece_vectors();
            } // Sort all of the evaluations the possible moves create
        }
        // Find and return the best candidate
        if (!bestLineCandidates.empty()) {
            auto bestCandidate = bestLineCandidates.at(0);
            for (int i = 1; i < bestLineCandidates.size() - 1; i++) {
                if (bestLineCandidates.at(i) > bestCandidate) {
                    bestCandidate = bestCandidate;
                }
            }
            return bestCandidate;
        } else {
            return {};
        }
    } else if (moveMaker == NERO) {
        for (auto &square: engine.blackPiecesSquares) {
            for (const auto &currentMove: engine.simulate_piece(square.coords)) {
                // Make the move
                beingConstructed.push_move(currentMove);
                engine.submit_matrix_notation(currentMove, NERO);
                engine.refresh_piece_vectors();

                bestLineCandidates.push_back(find_best_line(depth - 1, BIANCO, beingConstructed));

                // Cleanup
                beingConstructed.pop_move();
                engine.undo_move(currentMove);
                engine.refresh_piece_vectors();
            } // Sort all of the evaluations the possible moves create
        }
        if (!bestLineCandidates.empty()) {
            ApiumLine bestCandidate = bestLineCandidates.at(0);
            for (int i = 1; i < bestLineCandidates.size() - 1; i++) {
                if (bestLineCandidates.at(i) < bestCandidate) {
                    bestCandidate = bestLineCandidates.at(i);
                }
            }
            return bestCandidate;
        } else {
            return {};
        }
    }
}

