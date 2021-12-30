#include "gamehandler.h"

void GameHandler::cli_error(cliCase error) {
    switch (error) {
        case TOO_MANY_ARGUMENTS:
            std::cout << ERROR_COLOR << "Troppi argomenti" << RESET;
            RenderV2::cli_help_page();
        case WRONG_ARGUMENT:
            std::cout << ERROR_COLOR << "Comando non valido" << RESET;
            RenderV2::cli_help_page();
    }
}

int GameHandler::two_player_game(GameEngine& engine) {
    // Assumes that GameEngine has already been initialized
    PlayerColor current_color = BIANCO;
    MoveData issue;

    engine.render.render_board(engine.board, BIANCO);

    Move move = Move(BIANCO);

    // Game over is used to end the game, this can be done by using a command or by reaching a certain position
    while (engine.game_over() == GAME_NOT_OVER) {
        move.playerColor = current_color;

        // Get player input (move/command) and handle syntax errors
        issue = UI::get_move(move, engine, current_color);

        // If issue is valid the move is not a command and does not have syntax errors
        // If submit returns ALL_GOOD the move didn't have any semantic errors and was executed
        while (issue != VALID || engine.submit(move) != ALL_GOOD) {
            // If issue isn't INVALID then the move was a command
            if (issue != INVALID) {
                if (engine.execute_command(issue)) {
                    break;
                }
            }
            if (move.type.moveReturn == WHITE_RESIGN || move.type.moveReturn == BLACK_RESIGN) {
                engine.resign(move);
                return 1;
            } if (move.type.moveReturn == DRAW_OFFER) {
                // Someone has offered a draw
            }
            issue = UI::get_move(move, engine, current_color);
        }

        // See if a piece has gotten to the end, if so promote it
        engine.promote();

        // Renders looks for the last player's newest move for coloring, then switches the board to the opposite side
        engine.render.render_board(engine.board, current_color, move);

        // Switch player every turn, used both for colors and for move logs
        switch (current_color) {
            case BIANCO:
                current_color = engine.blackPlayer.color;
                break;
            case NERO:
                current_color = engine.whitePlayer.color;
                break;
            default:
                break;
        }
        engine.render.render_board(engine.board, current_color, move);
    }

    // When the game is over
    engine.render.render_board(engine.board, current_color);

    int whitePieces = engine.count_pieces(BIANCO);
    int blackPieces = engine.count_pieces(NERO);
    RenderV2::end_screen(whitePieces, blackPieces, engine.whitePlayer, engine.blackPlayer, engine.game_over(),
                             engine.start);
    return 0;
}

void GameHandler::debug(GameEngine &engine) {
    // Requires empty Engine initialization
    Player currentPlayer;
    currentPlayer.color = NERO;

    engine.board.edit(Coords(D, 4), Piece(BIANCO, DAMA));
    engine.board.edit(Coords(C, 3), Piece(NERO, DAMA));
    engine.board.edit(Coords(D, 4), Piece(BIANCO, DAMA));
    engine.board.edit(Coords(F, 6), Piece(BIANCO, DAMA));
    engine.render.render_board(engine.board, BIANCO);

    Move move (Coords(C, 2), Coords(D, 3), EAT);
    move.add_coords(Coords(F, 5));
    engine.recursive_check_eat(move);

// Main loop
    while (engine.game_over() == GAME_NOT_OVER) {
// Switch player every turn, used both for colors and for move logs
        switch (currentPlayer.color) {
            case BIANCO:
                currentPlayer = engine.blackPlayer;
                break;
            case NERO:
                currentPlayer = engine.whitePlayer;
                break;
            default:
                break;
        }
// Switch player colors
        Move move(currentPlayer.color);

// Get player input
        UI::get_move(move, engine, currentPlayer.color);

// If the move was invalid/was a command ask for another move
        while (engine.submit(move) != VALID) {
            if (move.type.moveReturn == WHITE_RESIGN || move.type.moveReturn == BLACK_RESIGN) {
                engine.resign(move);
                return;
            }
            UI::get_move(move, engine, currentPlayer.color);
        }

// See if a piece has gotten to the end, if so promote it
        engine.promote();

// Renders looks for the last player's newest move for coloring, then switches the board to the opposite side
        engine.render.render_board(engine.board, currentPlayer.color, move);
        engine.render.render_board(engine.board, currentPlayer.other_color(), move);
    }

// When the game is over
    engine.render.render_board(engine.board, currentPlayer.color);

    int whitePieces = engine.count_pieces(BIANCO);
    int blackPieces = engine.count_pieces(NERO);
    RenderV2::end_screen(whitePieces, blackPieces, engine.whitePlayer, engine.blackPlayer, engine.game_over(),
                             engine.start);
}