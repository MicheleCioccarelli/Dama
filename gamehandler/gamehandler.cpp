#include "gamehandler.h"

void GameHandler::cli_error(cliCase error) {
    switch (error) {
        case TOO_MANY_ARGUMENTS:
            std::cout << ERROR_COLOR << "Troppi argomenti" << RESET;
            HelpPages::cli_help_page();
        case WRONG_ARGUMENT:
            std::cout << ERROR_COLOR << "Comando non valido" << RESET;
            HelpPages::cli_help_page();
    }
}

void GameHandler::two_player_game(GameEngine& engine) {
    // Requires empty Engine initialization
    // Assumes that GameEngine has already been initialized
    PlayerColor current_color = BIANCO;
    MoveData issue;
    bool gameWasEndedByCommand = false;

    char choice {};

    engine.render.render_board(engine.board, BIANCO);

    Move move = Move(BIANCO);

    // Game over is used to end the game, this can be done by using a command or by reaching a certain position
    while (engine.game_over() == GAME_NOT_OVER) {
        move.playerColor = current_color;

        // Get player input (move/command) and handle syntax errors
        issue = UI::get_move(move, engine, current_color);

        // If issue is valid the move is not a command and does not have syntax errors
        // If submit returns ALL_GOOD the move didn't have any semantic errors and was executed
        while (issue != VALID || engine.submit(move, current_color) != ALL_GOOD) {
            // If issue isn't INVALID then the move was a command
            if (issue != INVALID) {
                if (CommandHandler::execute_command(engine, issue)) {
                    // Execute command handled formatting the end screen
                    gameWasEndedByCommand = true;
                    break;
                }
                if (issue == LOADBOARDPOS) {
                    std::cout << "Vuoi stampare la configurazione corrente? [s/n]\n";
                    std::cin >> choice;
                    std::cin.ignore(100, '\n');
                    if (choice == 's')
                        engine.render.render_board(engine.board, BIANCO);
                }
            }
            issue = UI::get_move(move, engine, current_color);
        }
        if (!gameWasEndedByCommand) {
            engine.board.promote();
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
        } else {
            std::cout << "Vuoi salvare la partita? [" << ACCEPT_DRAW << "s" << RESET << "/" << REFUSE_DRAW << "n" << RESET << "]\n> ";
            std::cin >> choice;
            std::cin.ignore(100, '\n');
            if (choice == 's') {
                CommandHandler::execute_command(engine, SAVE);
            }
            return;
        }
    }

    // When the game is over
    engine.render.render_board(engine.board, current_color);

    int whitePieces = engine.count_pieces(BIANCO);
    int blackPieces = engine.count_pieces(NERO);
    RenderV2::end_screen(whitePieces, blackPieces, engine.whitePlayer, engine.blackPlayer, engine.game_over(),
                         engine.start);
    std::cout << "Vuoi salvare la partita? [" << ACCEPT_DRAW << "s" << RESET << "/" << REFUSE_DRAW << "n" << RESET << "]\n> ";
    std::cin >> choice;
    std::cin.ignore(100, '\n');
    if (choice == 's') {
        CommandHandler::execute_command(engine, SAVE);
    }
}

void GameHandler::debug(GameEngine &engine) {
    // Requires empty Engine initialization
    // Assumes that GameEngine has already been initialized
    PlayerColor current_color = BIANCO;
    MoveData issue;
    bool gameWasEndedByCommand = false;

    // OH BOY
    engine.board.edit_human_notation(Coords(A, 7), Piece(BIANCO, DAMA));
    engine.board.edit_human_notation(Coords(B, 6), Piece(BIANCO, DAMA));
    engine.board.edit_human_notation(Coords(C, 7), Piece(BIANCO, DAMA));
    engine.board.edit_human_notation(Coords(D, 6), Piece(BIANCO, DAMA));
    engine.board.edit_human_notation(Coords(E, 7), Piece(BIANCO, DAMA));
    engine.board.edit_human_notation(Coords(F, 6), Piece(BIANCO, DAMA));
    engine.board.edit_human_notation(Coords(G, 7), Piece(BIANCO, DAMA));
    engine.board.edit_human_notation(Coords(H, 6), Piece(BIANCO, DAMA));
    engine.board.edit_human_notation(Coords(B, 8), Piece(NERO, DAMA));
    engine.board.edit_human_notation(Coords(D, 8), Piece(NERO, DAMA));
    engine.board.edit_human_notation(Coords(F, 8), Piece(NERO, DAMA));
    engine.board.edit_human_notation(Coords(H, 8), Piece(NERO, DAMA));

    engine.render.render_board(engine.board, current_color);
    Move move = Move(BIANCO);
    int i = 0;
    // Game over is used to end the game, this can be done by using a command or by reaching a certain position
    while (engine.game_over() == GAME_NOT_OVER) {
        move.playerColor = current_color;
        std::vector<Move> movesFound = engine.simulate_piece(Coords(A, 4));

        // Get player input (move/command) and handle syntax errors
        issue = UI::get_move(move, engine, current_color);
        // If issue is valid the move is not a command and does not have syntax errors
        // If submit returns ALL_GOOD the move didn't have any semantic errors and was executed
        while (issue != VALID || engine.submit(move, current_color) != ALL_GOOD) {
            // If issue isn't INVALID then the move was a command
            if (issue != INVALID) {
                if (CommandHandler::execute_command(engine, issue)) {
                    // Execute command handled formatting the end screen
                    gameWasEndedByCommand = true;
                    break;
                }
            }
            issue = UI::get_move(move, engine, current_color);
        }
        if (!gameWasEndedByCommand) {
            // Renders looks for the last player's newest move for coloring, then switches the board to the opposite side
            engine.render.render_board(engine.board, current_color, move);
            if (i == 1) {

                i = 0;
            } else {
                i++;
                std::cout << i << std::endl;
            }

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
        } else {
            return;
        }
    }

        // When the game is over
        engine.render.render_board(engine.board, current_color);

        int whitePieces = engine.count_pieces(BIANCO);
        int blackPieces = engine.count_pieces(NERO);
        RenderV2::end_screen(whitePieces, blackPieces, engine.whitePlayer, engine.blackPlayer, engine.game_over(),
                             engine.start);
}