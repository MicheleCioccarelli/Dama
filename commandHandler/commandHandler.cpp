#include "commandHandler.h"


bool CommandHandler::execute_game_command(GameEngine& gameEngine, MoveData command) {
    char input;
    // Used by the SAVE command
    std::string boardPos{};
    std::string gameName {};
    std::string path {};
    // 0, 1, or 2. It will be saved in the file
    int deducedState {};
    GameState gameState;

    // Returns true if the game should be ended
    switch (command) {
        case HELP_PAGE:
            HelpPages::help_page();
            return false;
        case COMMAND_HELP_PAGE:
            HelpPages::commands_help_page();
            return false;
        case MOVEORDER:
            FileHandler::print_move_sequence(gameEngine);
            return false;
        case SUMMARY:
            RenderV2::end_screen(gameEngine.count_pieces(BIANCO), gameEngine.count_pieces(NERO), gameEngine.whitePlayer, gameEngine.blackPlayer, GAME_NOT_OVER, gameEngine.start);
            return false;
        case WHITE_RESIGN:
            RenderV2::end_screen(gameEngine.count_pieces(BIANCO), gameEngine.count_pieces(NERO),
                                 gameEngine.whitePlayer, gameEngine.blackPlayer, WHITE_RESIGNED, gameEngine.start);
            return true;
        case BLACK_RESIGN:
            RenderV2::end_screen(gameEngine.count_pieces(BIANCO), gameEngine.count_pieces(NERO),
                                 gameEngine.whitePlayer, gameEngine.blackPlayer, BLACK_RESIGNED, gameEngine.start);
            return true;
        case W_DRAW_OFFER:
            std::cout << gameEngine.blackPlayer.name << " accetta il pareggio? [" << ACCEPT_DRAW << "s" << RESET << "/" << REFUSE_DRAW << "n"
                      << RESET << "]\n";
            std::cin >> input;
            if (input == 's') {
                RenderV2::end_screen(gameEngine.count_pieces(BIANCO), gameEngine.count_pieces(NERO),
                                     gameEngine.whitePlayer, gameEngine.blackPlayer, DRAW, gameEngine.start);
                return true;
            } else {
                std::cout << "Niente pareggio\n" << std::endl;
                std::cin.ignore(1);
                return false;
            }
        case B_DRAW_OFFER:
            std::cout << gameEngine.blackPlayer.name << " accetta il pareggio? [" << ACCEPT_DRAW << "s" << RESET << "/" << REFUSE_DRAW << "n"
                      << RESET << "]\n";
            std::cin >> input;
            if (input == 's') {
                RenderV2::end_screen(gameEngine.count_pieces(BIANCO), gameEngine.count_pieces(NERO),
                                     gameEngine.whitePlayer, gameEngine.blackPlayer, DRAW, gameEngine.start);
                return true;
            } else {
                std::cout << "Niente pareggio" << std::endl;
                std::cin.ignore(1);
                return false;
            }
        case LOADBOARDPOS:
            std::cout << "Scrivi la nuova configurazione della scacchiera usando la notazione BoardPos [" << REFUSE_DRAW << "q" << RESET << " per uscire]\n";
            std::cout << RED << "Se la notazione contiene errori la scacchiera potrebbe corrompersi\n" << RESET;

            getline(std::cin, boardPos);

            if (boardPos == "q") {
                return false;
            }
            if (!BoardPos::notation_to_board(boardPos, gameEngine.board)) {
                std::cout << "La notazione scritta conteneva errori :(\n";
            }
            return false;
        case GENERATEBOARDPOS:
                std::cout << BoardPos::board_to_noation_ignoring_white_squares(gameEngine.board) << "\n";
            return false;
        case SAVE:
            // If at any point the user enters q we should quit
            std::cout << "Che nome vuoi dare alla partita? [" << RED << "q" << RESET << " per uscire]\n> ";
            getline(std::cin, gameName);
            if (gameName == "q") {
                std::cout << "\nChiuso il menu di salvataggio";
                return false;
            }
            std::cout << "Dove vuoi che il file sia salvato [non scrivere nessuna path per salvare nella directory dell'eseguibile]\n> ";
            getline(std::cin, path);
            if (path == "q") {
                std::cout << "\nChiuso il menu di salvataggio";
                return false;
            }
            // Determine the current state of the game;
            gameState = gameEngine.game_over();
            switch(gameState) {
                case GAME_NOT_OVER:
                    // The game is still going on
                    deducedState = 0;
                    break;
                case WHITE_WIN:
                case BLACK_RESIGNED:
                    // White won
                    deducedState = 1;
                    break;
                case BLACK_WIN:
                case WHITE_RESIGNED:
                    // Black won
                    deducedState = 2;
                    break;
                case STALEMATE:
                case DRAW:
                    deducedState = 3;
                default:
                    deducedState = 4;
            }
            if (FileHandler::create_file(gameEngine, gameName, path, deducedState) == OPENING_ISSUE) {
                std::cout << ERROR_COLOR << "C'è stato un errore a creare il file [path sbagliata?]\n" << RESET;
                return false;
            }
            return false;
        case QUIT:
            return true;
    }
    return false;
}