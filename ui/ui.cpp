#include "ui.h"

void UI::init(GameEngine &engine) {
    std::string playerName;

    std::cout << MAG << "Benvenuto nella dama" << RESET << std::endl << std::endl;;

    //std::cout << "Benvenuto nella dama" << std::endl << std::endl;

    std::cout << "Chi gioca bianco?" << std::endl;
    getline(std::cin, playerName);
    if (playerName.size() <= 30) {
        engine.whitePlayer.name = playerName;
        engine.whitePlayer.color = BIANCO;
    }

    std::cout << "Chi gioca nero?" << std::endl;
    getline(std::cin, playerName);
    if (playerName.size() <= 30) {
        engine.blackPlayer.name = playerName;
        engine.blackPlayer.color = NERO;
    }
}

void UI::command_to_move(const std::vector<Command>& commands, Move &move) {
    for (int i = 0; i < commands.size(); i++) {
        switch (commands[i].type.moveType) {
            case MOVE:
                move.type.moveType = MOVE;
                if (move.type.moveReturn != BLOWABLE) {
                    move.type.moveReturn = commands[i].type.moveReturn;
                }
                move.add_coords(commands[i].startingCoords);
                move.add_coords(commands[i].endingCoords);
                i = commands.size();
                break;
            case EAT:
                move.type.moveType = EAT;
                if (move.type.moveReturn != BLOWABLE) {
                    move.type.moveReturn = commands[i].type.moveReturn;
                }
                for (int j = 0; j < commands[i].eatenCoords.size(); j++) {
                    move.coords.push_back(commands[i].eatenCoords[j]);
                }
                i = commands.size();
                break;
        }
        switch (commands[i].type.moveReturn) {
            case BLOWABLE:
                move.blownCoord = commands[i].blownCoords;
                move.type.moveReturn = BLOWABLE;
                break;
            case TOO_SHORT:
                move.type.moveReturn = TOO_SHORT;
            case WRONG_OPERATOR:
                move.type.moveReturn = WRONG_OPERATOR;
        }
    }
}

MoveReturn UI::validate_input(const Coords &coords) {
    if (coords.column == Z) {
        return MISINPUT;
    } else if (coords.row < 0 || coords.row > 8) {
        return MISINPUT;
    } else {
        return VALID;
    }
}

MoveReturn UI::validate_command(const std::vector<Command>& commands) {
    for (int i = 0; i < commands.size(); i++) {
        switch (commands[i].type.moveType) {
            case MOVE:
                if (UI::validate_input(commands[i].startingCoords) == MISINPUT) {
                    return MISINPUT;
                } else if (UI::validate_input(commands[i].endingCoords) == MISINPUT) {
                    return MISINPUT;
                }
                break;
            case EAT:
                for (int j = 0; j < commands[i].eatenCoords.size(); j++) {
                    if (validate_input(commands[i].eatenCoords[j]) == MISINPUT) {
                        return MISINPUT;
                    }
                }
                break;
        }
        if (commands[i].type.moveReturn == BLOWABLE) {
            if (UI::validate_input(commands[i].blownCoords) == MISINPUT) {
                return MISINPUT;
            }
        }
    }
    return VALID;
}

MoveReturn UI::check_color(Move &move, PlayerColor currentPlayer, GameEngine& engine) {
    switch (currentPlayer)  {
        case BIANCO:
            if (engine.deduce_color(move) == NERO) {
                if (move.type.moveReturn != BLOWABLE) {
                    move.type.moveReturn = WRONG_COLOR;
                }
            }
            break;
        case NERO:
            if (engine.deduce_color(move) == BIANCO) {
                if (move.type.moveReturn != BLOWABLE) {
                    move.type.moveReturn = WRONG_COLOR;
                }
            }
            break;
        default:
            return UNDEFINED;
    }
}

MoveReturn UI::get_move(Move& move, GameEngine& engine, PlayerColor currentPlayer) {
    std::vector<std::string> input;
    std::vector<Command> commands;

    move = Move(currentPlayer);

    std::string a;

    if (currentPlayer == BIANCO) {
        std::cout << "Mossa di " << PLAYER_COLOR << engine.whitePlayer.name << RESET << " > ";
    } else if (currentPlayer == NERO) {
        std::cout << "Mossa di " << PLAYER_COLOR << engine.blackPlayer.name << RESET << " > ";
    }
    getline(std::cin, a);
    std::stringstream stream(a);

    // Initializing the strings
    input.emplace_back("~");
    input.emplace_back("~");

    for (int i = 0; i < MAX_COMMANDS; i++) {
        stream >> input[i];
        if (input[i] == "~") {
            break;
        }
        if (input[i].size() < 5 && input[i] != "~") {
            move.type = TOO_SHORT;
            return TOO_SHORT;
        }
        if (input[i] == "aiuto") {
            move.type = HELP_PAGE;
            engine.render.help_page();
            return HELP_PAGE;
        }

        std::transform(input[i].begin(), input[i].end(), input[i].begin(), ::toupper);
        commands.emplace_back(input[i], engine);
    }

    // Proceed with the program only if the coordinates are right, this should avoid segfault
    if (validate_command(commands) == VALID) {
        command_to_move(commands, move);
    } else {
        move.type.moveReturn = MISINPUT;
        UI::log_error(MISINPUT);
        return MISINPUT;
    }

    UI::check_color(move, currentPlayer, engine);
    return VALID;
}

void UI::log_error(MoveReturn error) {
    switch (error) {
        case VALID:
            return;
        case WHITE_SQUARE:

            std::cout << ERROR_COLOR << "Ti stai muovendo in una casella bianca, scrivi aiuto per informazioni" << RESET;
            break;
        case TOO_FAR:
            std::cout << ERROR_COLOR << "Non puoi muoverti di piu di una casella alla volta, scrivi aiuto per informazioni" << RESET;
            break;
        case BEHIND:
            std::cout << ERROR_COLOR << "La damina non puomuoversi indietro, scrivi aiuto per informazioni" << RESET;
            break;
        case POPULATED:
            std::cout << ERROR_COLOR << "La casella che vuoi occupare è occupata, scrivi aiuto per informazioni" << RESET;
            break;
        case EMPTY_START:
            std::cout << ERROR_COLOR << "La casella di partenza è vuota, scrivi aiuto per informazioni" << RESET;
            break;
        case EMPTY_TARGET:
            std::cout << ERROR_COLOR << "Non puoi mangiare il niente, scrivi aiuto per informazioni" << RESET;
            break;
        case CANNIBALISM:
            std::cout << ERROR_COLOR << "Non commettere cannibalismo, scrivi aiuto per informazioni" << RESET;
            break;
        case TOO_BIG:
            std::cout << ERROR_COLOR << "La tua damina è troppo ambiziosa, scrivi aiuto per informazioni" << RESET;
            break;
        case FRIENDLY_FIRE:
            std::cout << ERROR_COLOR << "Fuoco amico non attivo, scrivi aiuto per informazioni" << RESET;
            break;
        case INVALID:
            std::cout << ERROR_COLOR << "Qualcosa è andato storto, scrivi aiuto per informazioni" << RESET;
            break;
        case OUT_OF_BOUNDS:
            std::cout << ERROR_COLOR << "Andresti fuori dalla scacchiera, scrivi aiuto per informazioni" << RESET;
            break;
        case ROCK_SOLID:
            std::cout << ERROR_COLOR << "Non puoi soffiarla, scrivi aiuto per informazioni" << RESET;
            break;
        case UNDEFINED:
            std::cout << ERROR_COLOR << "Qualcosa è andato molto storto o hai soffiato e basta, scrivi aiuto per informazioni" << RESET;
            break;
        case TOO_SHORT:
            std::cout << ERROR_COLOR << "La mossa è troppo corta, scrivi aiuto per informazioni" << RESET;
            break;
        case WRONG_OPERATOR:
            std::cout << ERROR_COLOR << "Hai sbagliato operatore, prova a scrivere help, scrivi aiuto per informazioni" << RESET;
            break;
        case WRONG_COLOR:
            std::cout << ERROR_COLOR << "Stai provando a muovere pezzi del colore sbagliato, scrivi aiuto per informazioni" << RESET;
            break;
        case MISINPUT:
            std::cout << ERROR_COLOR << "La mossa messa in input è sbagliata, scrivi aiuto per informazioni" << RESET;
            break;
    }
    std::cout << std::endl;
}

void UI::start_game() {
    // Initialize board looks
    BoardTokens board(NORMAL);
    SetPieces pieces(NORMAL);
    BoardCoords coords(NORMAL);
    GameEngine engine(STANDARD, board, pieces, coords, 8, 8);

    PlayerColor currentPlayer = NERO;

    UI::init(engine);
    // Main loop
    while (engine.game_over(8, 8) == GOOD) {
        // Switch playerColor every turn
        switch (currentPlayer) {
            case BIANCO:
                currentPlayer = NERO;
                break;
                case NERO:
                    currentPlayer = BIANCO;
                    break;
        }

        engine.render.render_board((Player&)currentPlayer, engine.board, 8, 8);
        Move move(currentPlayer);

        // ========= GETTING INPUT ==========
        // get_move returns MISINPUT if the move provided is very wrong (e.g. A9-KH)
        while (UI::get_move(move, engine, currentPlayer) == MISINPUT) {
        }
        while (engine.submit(move) != VALID) {
            UI::get_move(move, engine, currentPlayer);
        }
        engine.promote(8, 8);
        engine.render.render_board((Player&)currentPlayer, engine.board, 8, 8);

    }

    engine.render.render_board((Player&)currentPlayer, engine.board, 8, 8);

    int whitePieces = engine.count_pieces(BIANCO, 8, 8);
    int blackPieces = engine.count_pieces(NERO, 8, 8);

    engine.render.end_screen(whitePieces, blackPieces, engine.whitePlayer, engine.blackPlayer,
                             engine.game_over(8, 8));
}
