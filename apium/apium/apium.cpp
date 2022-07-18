#include "apium.h"

Apium::Apium(GameEngine &engine)
    : m_engine(engine) {}

Apium::Apium(GameEngine &engine, Playstyle playstyle, PlayerColor apiumColor)
    : m_engine(engine), m_playstyle(playstyle), whoIsBeingPlayed(apiumColor) {}

Apium::Apium(Playstyle playStyle, PlayerColor apiumColor)
    : m_playstyle(playStyle), whoIsBeingPlayed(apiumColor) {}

void Apium::set_eval(float eval) {
    m_eval = eval;
}

void Apium::setEngine(const GameEngine &rhs) {
    m_engine = rhs;
}

void Apium::set_playstyle(const Playstyle playstyle) {
    m_playstyle = playstyle;
}

float Apium::get_eval() const { return m_eval; }

void Apium::update_eval() {
    set_eval(evaluate_current_position());
}

void Apium::sync_engine(const GameEngine &engine) {
    m_engine = engine;
}

float Apium::evaluate_piece(Coords pieceCoords, Piece piece) const {
    // Expects matrix-notation
    float pieceEval = 0;

    if (piece == Piece()) {
        piece = m_engine.board.matrix[pieceCoords.row][pieceCoords.column].piece;
    }

    // There was no piece in the position
    if (piece.type == VUOTA) {
        return pieceEval;
    }

     switch (m_playstyle) {
        case NEUTRAL:
        /* EVALUATION TABLE FOR NEUTRAL
         * DAMINA -> +1   | Increment based on position: +0.25
         * DAMONA -> +3   | Increment based on position: +0.25
         */
            if (piece.type == DAMA) {
                if (piece.color == BIANCO) {
                    // White damina
                    pieceEval++;

                    // Add .25 based on how close it is to being a damone (1 square closer = +.25)
                    pieceEval += pieceCoords.row * .25;
                } else {
                    // Black damina
                    pieceEval--;

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
    for (auto& square : m_engine.blackPiecesSquares)
                returnValue += evaluate_piece(square.coords, square.piece);
    // evaluate all the white pieces
    for (auto& square : m_engine.whitePiecesSquares)
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
    float bestEval = m_eval;
    float newEval = bestEval;
    Move bestMove;
    // The first time the loop runs a move gets assigned to bestMove, so that if all the moves suck the function
    // doesn't return an empty move
    bool firstLoop = true;

    if (whoIsPlaying == BIANCO) {
        // White is trying to maximize the eval and moves the white pieces
        for (auto &square: m_engine.whitePiecesSquares) { // For each white piece on the board
            for (const auto &currentMove: m_engine.simulate_piece(square.coords)) {
                // For each of the moves the piece could make
                if (firstLoop) {
                    firstLoop = false;
                    bestMove = currentMove;
                }
                m_engine.submit_matrix_notation(currentMove, whoIsPlaying); // play currentMove

                newEval = evaluate_current_position();
                if (newEval > bestEval) {
                    bestEval = newEval;
                    bestMove = currentMove;
                }
                m_engine.undo_move(currentMove); // cleanup after you played currentMove
                m_engine.refresh_piece_vectors();
            }
        }
    } else {
        // Black wants to minimize the score and plays with the black pieces
        for (auto& square : m_engine.blackPiecesSquares) { // For each black piece on the board
            for (const auto& currentMove : m_engine.simulate_piece(square.coords)) {
                // For each of the moves the piece could make
                if (firstLoop) {
                    firstLoop = false;
                    bestMove = currentMove;
                }
                m_engine.submit_matrix_notation(currentMove, whoIsPlaying); // play currentMove

                newEval = evaluate_current_position();
                if (newEval < bestEval) {
                    bestEval = newEval;
                    bestMove = currentMove;
                }
                m_engine.undo_move(currentMove); // cleanup after you played currentMove
                m_engine.refresh_piece_vectors();
            }
        }
    }
    return bestMove;
}

//void Apium::find_best_line(int depth, PlayerColor moveMaker, ApiumLine beingConstructed) {
//    if (depth == 0) {
//        // This is the end of this recursive branch, evaluate where beingConstructed brings you
//        float currentEval = evaluate_current_position();
//
//        if (!bestLine.get_moves().empty()) {
//            // The check above is to avoid ending up without any moves to make if everything sucks
//            if (this->whoIsBeingPlayed == BIANCO) {
//                // Maximizing the score
//                if (currentEval > bestLine.get_eval()) {
//                    bestLine = beingConstructed;
//                    bestLine.set_eval(currentEval);
//                }
//            } else {
//                // Minimize the score
//                if (currentEval < bestLine.get_eval()) {
//                    bestLine = beingConstructed;
//                    bestLine.set_eval(currentEval);
//                }
//            }
//        } else {
//            bestLine = beingConstructed;
//            bestLine.set_eval(currentEval);
//        }
//        return;
//    }
//    if (moveMaker == BIANCO) {
//        for (auto& square : m_engine.whitePiecesSquares) { // For each white piece on the board
//            for (const auto& currentMove : m_engine.simulate_piece(square.coords)) {
//                // Add a move to the line being created
//                beingConstructed.push_move(currentMove);
//                // Perform the move
//                m_engine.submit_matrix_notation(currentMove, BIANCO);
//                m_engine.refresh_piece_vectors();
//
//                // Continue the recursion
//                find_best_line(depth - 1, NERO, beingConstructed);
//                // All the branches regarding currentMove have been explored, cleanup for the next move
//                beingConstructed.pop_move();
//                m_engine.undo_move(currentMove);
//                m_engine.refresh_piece_vectors();
//            }
//        }
//    } else { // Same as above, just for black
//        for (auto& square : m_engine.blackPiecesSquares) {
//            for (const auto& currentMove : m_engine.simulate_piece(square.coords)) {
//
//                beingConstructed.push_move(currentMove);
//
//                m_engine.submit_matrix_notation(currentMove, NERO);
//                m_engine.refresh_piece_vectors();
//
//                find_best_line(depth - 1, BIANCO, beingConstructed);
//
//                beingConstructed.pop_move();
//                m_engine.undo_move(currentMove);
//                m_engine.refresh_piece_vectors();
//            }
//        }
//    }
//}

/**
 * Idea for find_best_line()'s redesign:
 * First, you generate a tree of all the possible position that can be reached in a certain number of moves.
 * Once you get to the end you can start deciding which move is best out of the ones available
 * */

ApiumLine Apium::find_best_line(int depth, PlayerColor moveMaker, ApiumLine beingConstructed) {
    if (depth == 0) {
        return {beingConstructed.get_moves(), evaluate_current_position()};
    }
    // The moves which can be made at this point of the recursion
    std::vector<ApiumLine> bestLineCandidates;
    float currentEval {};

    if (moveMaker == BIANCO) {
        for (auto &square: m_engine.whitePiecesSquares) { // For each white piece on the board
            for (const auto &currentMove: m_engine.simulate_piece(square.coords)) { // For each move a piece can make
                // Make the move
                beingConstructed.push_move(currentMove);
                m_engine.submit_matrix_notation(currentMove, BIANCO);
                m_engine.refresh_piece_vectors();

                // Add to the candidates the move that was just checked
                bestLineCandidates.push_back(find_best_line(depth - 1, NERO, beingConstructed));

                // Cleanup
                beingConstructed.pop_move();
                m_engine.undo_move(currentMove);
                m_engine.refresh_piece_vectors();
            } // Sort all of the evaluations the possible moves create
        }
        return *std::max_element(bestLineCandidates.begin(), bestLineCandidates.end());

    } else if (moveMaker == NERO) {
        for (auto &square: m_engine.blackPiecesSquares) {
            for (const auto &currentMove: m_engine.simulate_piece(square.coords)) {
                // Make the move
                beingConstructed.push_move(currentMove);
                m_engine.submit_matrix_notation(currentMove, NERO);
                m_engine.refresh_piece_vectors();

                bestLineCandidates.push_back(find_best_line(depth - 1, BIANCO, beingConstructed));

                // Cleanup
                beingConstructed.pop_move();
                m_engine.undo_move(currentMove);
                m_engine.refresh_piece_vectors();
            } // Sort all of the evaluations the possible moves create
        }
        return *std::min_element(bestLineCandidates.begin(), bestLineCandidates.end());
    }
}

