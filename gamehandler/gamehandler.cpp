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

int GameHandler::play_back(const std::string& gameLocation) {
    std::ifstream i_file {gameLocation};
    std::string whitePlayerName {};
    std::string blackPlayerName {};
    std::string beingProcessed {};
    std::string gameName {};

    if (!i_file) {
        std::cout << ERROR_COLOR << "Problem opening file\n" << RESET;
        return 1;
    }
    // Get white player and black player names
    getline(i_file, beingProcessed);
    // Get the position of first occurrence of :
    std::size_t pos = beingProcessed.find(':');
    // Check if index position is valid
    if (pos != std::string::npos) {
        // Remove all characters before :
        beingProcessed = beingProcessed.substr(pos+1);
    }
    blackPlayerName = beingProcessed;

    // Do the same but for the black player
    getline(i_file, beingProcessed);
    pos = beingProcessed.find(':');
    if (pos != std::string::npos) {
        beingProcessed = beingProcessed.substr(pos+1);
    }
    whitePlayerName = beingProcessed;

    // Get the game name
    getline(i_file, beingProcessed);
    pos = beingProcessed.find(':');
    if (pos != std::string::npos) {
        beingProcessed = beingProcessed.substr(pos+1);
    }
    gameName = beingProcessed;
    ///home/sedano/CLionProjects/Dama/playbacktest1.dama
    // Fetch the game result
    int gameResult;
    getline(i_file, beingProcessed);
    // Get the position of first occurrence of :
    pos = beingProcessed.find(':');
    if (pos != std::string::npos) {
        beingProcessed = beingProcessed.substr(pos+1);
    }
    // Convert the string to input
    gameResult = std::stoi(beingProcessed);

    std::string gameNotation{};
    getline(i_file, beingProcessed);
    // Get the position of first occurrence of :
    pos = beingProcessed.find(':');
    if (pos != std::string::npos) {
        beingProcessed = beingProcessed.substr(pos+1);
    }
    gameNotation = beingProcessed;
    // Convert all the [moves][] from the .dama file into an actual squence of moves
    std::vector<Move> movesPlayed = FileHandler::notation_to_moves(gameNotation);

    // Engine to acutally make and render the moves
    GameEngine engine;
    PlayerColor playerPlaying = NERO;
    // The side from which you render the board
    PlayerColor playerPOV = BIANCO;

    std::string input {};
    std::cout << MAG << "Premi invio per continuare" << RESET << std::endl;
    getline(std::cin, input);

    for (const Move& move : movesPlayed) {
        printTheMove:
        if (input == "") {
            // The user wants to proceed with the game
            if (playerPlaying == NERO) {
                playerPlaying = BIANCO;
            } else {
                playerPlaying = NERO;
            }
            engine.submit_human_notation(move, playerPlaying);

            engine.render.render_board(engine.board, playerPOV, move);
            if (playerPlaying == BIANCO) {
                std::cout << PLAYER_COLOR << whitePlayerName;
            } else {
                std::cout << PLAYER_COLOR << blackPlayerName;
            }
            std::cout << RESET << ": " << move.toStringMatrix() << "\n>"; //playbacktest1.dama
            getline(std::cin, input);
        } else {
            while (input != "") {
                // The user typed a command or something wrong
                std::transform(input.begin(), input.end(), input.begin(), ::toupper); // Make everything uppercase
                // Check for commands
                if (input == "AIUTO") {
                    HelpPages::playback_help_page();
                } else if (input == "SWITCH") {
                    if (playerPOV == NERO) {
                        playerPOV = BIANCO;
                    } else {
                        playerPOV = NERO;
                    }
                } else if (input == "Q" || input == "QUIT") {
                    i_file.close();
                    return 1;
                } else { // playbacktest1.dama
                    // There was a mistype
                    std::cout << ERROR_COLOR
                    << "input invalido, scrivi aiuto per informazioni (premi invio per andare alla mossa successiva)\n"<< RESET;
                }
                // Ask for another input
                std::cout << ">";
                getline(std::cin, input);
                goto printTheMove; // :)
            }
        }
    }
    RenderV2::end_screen(engine.count_pieces(BIANCO), engine.count_pieces(NERO), engine.whitePlayer, engine.blackPlayer, engine.game_over(),
                         engine.start);
    i_file.close();
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
        // If submit_human_notation returns ALL_GOOD the move didn't have any semantic errors and was executed
        while (issue != VALID || engine.submit_human_notation(move, current_color) != ALL_GOOD) {
            // If issue isn't INVALID then the move was a command
            if (issue != INVALID) {
                if (CommandHandler::execute_game_command(engine, issue)) {
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
                CommandHandler::execute_game_command(engine, SAVE);
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
        CommandHandler::execute_game_command(engine, SAVE);
    }
}

void GameHandler::debug(GameEngine &engine) {
    // Requires empty Engine initialization
    // Assumes that GameEngine has already been initialized
    engine.board.edit_human_notation(Coords(E, 5), Piece(NERO, DAMA));
    engine.board.edit_human_notation(Coords(C, 5), Piece(NERO, DAMA));
    engine.board.edit_human_notation(Coords(G, 7), Piece(NERO, DAMA));
    engine.board.edit_human_notation(Coords(H, 8), Piece(NERO, DAMA));
    engine.board.edit_human_notation(Coords(D, 4), Piece(BIANCO, DAMA));

//    engine.board.edit_human_notation(Coords(F, 8), Piece(BIANCO, DAMONE));
//    engine.board.edit_human_notation(Coords(H, 8), Piece(NERO, DAMA));
//    engine.board.edit_human_notation(Coords(H, 6), Piece(NERO, DAMA));
//    engine.board.edit_human_notation(Coords(E, 3), Piece(NERO, DAMA));

    engine.refresh_piece_vectors();

    engine.render.render_board(engine.board, BIANCO);

    Apium apium(engine,NEUTRAL, BIANCO);

    apium.sync_engine(engine);

    apium.minimax(5, BIANCO);

    auto line = apium.bestLine;

    // Make Apium play against itself





    PlayerColor color = NERO;
    for (auto& move : line.get_moves()) {
        color == NERO ? color = BIANCO : color = NERO;
        engine.precise_promote(move);
        engine.submit_matrix_notation(move, color);
        move.convert_all(false);
        engine.render.render_board(engine.board, color, move);
    }
}