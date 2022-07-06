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

// For each child of position
ApiumLine Apium::minimax(int depth, float alpha, float beta, bool maximizingPlayer, std::vector<Move> currentLine) {
    if (depth == 0 || m_engine.game_over() != GAME_NOT_OVER) {
        // Needs to be tested, assigns the current best line to the one which will be played
        if (evaluate_current_position() > bestLine.get_eval() && whoIsBeingPlayed == BIANCO) {
            bestLine.set_eval(evaluate_current_position());
            bestLine.set_moves(currentLine);
        } else if (evaluate_current_position() < bestLine.get_eval() && whoIsBeingPlayed == NERO) {
            bestLine.set_eval(evaluate_current_position());
            bestLine.set_moves(currentLine);
        }
        // Return the static evaluation of the current position
        return {currentLine, evaluate_current_position()};
    }
    float eval, maxEval, minEval;

    // If the maximazing player is playing (WHITE)
    if (maximizingPlayer) {
        alpha = MAX_EVAL;
        // For each child of the position
        /**
         * Pick one of your pieces and generate all the moves it could make, then play them
         * one by one and evaluate all the positions, this could be optimized because the moves have to be
         * performed both when they are generated and when they are evaluated
         */
        for (const Square& whitePieceSquare : m_engine.whitePiecesSquares) { // For each white piece on the board
            for (Move& currentMove : m_engine.simulate_piece(whitePieceSquare.coords)) { // generate all of its possible moves
                // Evaluate them and repeat
                m_engine.board.execute_move(currentMove);
                m_engine.refresh_piece_vectors();
#ifdef DEBUG
                m_engine.render.render_board(m_engine.board, BIANCO, currentMove);
#endif
                currentLine.push_back(currentMove);
                eval = minimax(depth - 1, alpha, beta, false, currentLine).get_eval();

                m_engine.undo_move(currentMove);
                currentLine.pop_back();

                maxEval = std::max(maxEval, eval);
                alpha = std::max(alpha, eval);
                if (beta <= alpha) {
                    goto OUT_OF_THE_LOOP; // Used to break out of both loops
                }
            }
        }
        OUT_OF_THE_LOOP:
        return {currentLine, maxEval};

    } else {
        // If the minimazing player is playing (BLACK)
        beta = MIN_EVAL;
        for (const Square& blackPieceSquare : m_engine.blackPiecesSquares) { // For each white piece on the board
            for (Move& currentMove : m_engine.simulate_piece(blackPieceSquare.coords)) { // generate all of its possible moves
                // Evaluate them and repeat
                m_engine.board.execute_move(currentMove);
                m_engine.refresh_piece_vectors();

#ifdef DEBUG
                m_engine.render.render_board(m_engine.board, NERO, currentMove);
#endif

                currentLine.push_back(currentMove);
                eval = minimax(depth - 1, alpha, beta, true, currentLine).get_eval();

                m_engine.undo_move(currentMove);
                currentLine.pop_back();

                minEval = std::min(minEval, eval);
                beta = std::min(beta, eval);
                if (beta <= alpha) {
                    break;
                }
            }
        }
    }
    // Needs to be tested, assigns the current best line to the one which will be played
    if (evaluate_current_position() > bestLine.get_eval() && whoIsBeingPlayed == BIANCO) {
        bestLine.set_eval(evaluate_current_position());
        bestLine.set_moves(currentLine);
    } else if (evaluate_current_position() < bestLine.get_eval() && whoIsBeingPlayed == NERO) {
        bestLine.set_eval(evaluate_current_position());
        bestLine.set_moves(currentLine);
    }
    return {currentLine, minEval};
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
                m_engine.submit(currentMove, whoIsPlaying); // play currentMove

                newEval = evaluate_current_position();
                if (newEval > bestEval) {
                    bestEval = newEval;
                    bestMove = currentMove;
                }
                if (shouldCleanup) {
                    m_engine.undo_move(currentMove); // cleanup after you played currentMove
                    m_engine.refresh_piece_vectors();
                }
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
                m_engine.submit(currentMove, whoIsPlaying); // play currentMove

                newEval = evaluate_current_position();
                if (newEval < bestEval) {
                    bestEval = newEval;
                    bestMove = currentMove;
                }
                if (shouldCleanup) {
                    m_engine.undo_move(currentMove); // cleanup after you played currentMove
                    m_engine.refresh_piece_vectors();
                }
            }
        }
    }
    return bestMove;
}

ApiumLine Apium::find_best_line(int depth) {
    ApiumLine beingConstructed;
    for (; depth > 0 || m_engine.game_over() != GAME_NOT_OVER; depth--) { // Go until the game is over or deptg is 0
        beingConstructed.push_move(find_best_move(whoIsBeingPlayed, false));
        // Switch player colors
        whoIsBeingPlayed == BIANCO ? whoIsBeingPlayed = NERO : whoIsBeingPlayed = BIANCO;
    }

    beingConstructed.set_eval(evaluate_current_position());

    // Cleanup
    for (int i = beingConstructed.get_moves().size()-1; i >= 0; i--) {
        m_engine.undo_move(beingConstructed.get_moves().at(i));
    }
    m_engine.refresh_piece_vectors(); // The code right above might crash

    return beingConstructed;
}