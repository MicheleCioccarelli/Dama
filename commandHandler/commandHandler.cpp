#include "commandHandler.h"


bool CommandHandler::execute_command(GameEngine& engine, MoveData command) {
    char input;
    // Returns true if the game should be ended
    switch (command) {
        case HELP_PAGE:
            RenderV2::help_page();
            return false;
        case SUMMARY:
            RenderV2::end_screen(engine.count_pieces(BIANCO), engine.count_pieces(NERO), engine.m_whitePlayer, engine.m_blackPlayer, GAME_NOT_OVER, engine.m_start);
            return false;
        case WHITE_RESIGN:
            RenderV2::end_screen(engine.count_pieces(BIANCO), engine.count_pieces(NERO),
                                 engine.m_whitePlayer, engine.m_blackPlayer, WHITE_RESIGNED, engine.m_start);
            return true;
        case BLACK_RESIGN:
            RenderV2::end_screen(engine.count_pieces(BIANCO), engine.count_pieces(NERO),
                                 engine.m_whitePlayer, engine.m_blackPlayer, BLACK_RESIGNED, engine.m_start);
            return true;
        case W_DRAW_OFFER:
            std::cout << engine.m_blackPlayer.name << " accetta il pareggio? [" << ACCEPT_DRAW << "s" << RESET << "/" << REFUSE_DRAW << "n"
                      << RESET << "]\n";
            std::cin >> input;
            if (input == 's') {
                RenderV2::end_screen(engine.count_pieces(BIANCO), engine.count_pieces(NERO),
                                     engine.m_whitePlayer, engine.m_blackPlayer, DRAW, engine.m_start);
                return true;
            } else {
                std::cout << "Niente pareggio\n" << std::endl;
                std::cin.ignore(1);
                return false;
            }
        case B_DRAW_OFFER:
            std::cout << engine.m_blackPlayer.name << " accetta il pareggio? [" << ACCEPT_DRAW << "s" << RESET << "/" << REFUSE_DRAW << "n"
                      << RESET << "]\n";
            std::cin >> input;
            if (input == 's') {
                RenderV2::end_screen(engine.count_pieces(BIANCO), engine.count_pieces(NERO),
                                     engine.m_whitePlayer, engine.m_blackPlayer, DRAW, engine.m_start);
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