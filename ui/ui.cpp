#include "ui.h"

void UI::init(GameEngine &engine) {
    std::string playerName;

    std::cout << MAG << "Benvenuto nella dama" << RESET << std::endl << std::endl;;

    std::cout << "Chi gioca bianco?" << std::endl;
    getline(std::cin, playerName);
    if (playerName.size() <= 30) {
        engine.whitePlayer.name = playerName;
        engine.whitePlayer.color = BIANCO;
    } else {
        engine.whitePlayer.name = "Nope";
        engine.whitePlayer.color = BIANCO;
    }

    std::cout << "Chi gioca nero?" << std::endl;
    getline(std::cin, playerName);
    if (playerName.size() <= 30) {
        engine.blackPlayer.name = playerName;
        engine.blackPlayer.color = NERO;
    } else {
        engine.blackPlayer.name = "Nope";
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

MoveReturn UI::check_input(const std::string &input) {
    // User typed spaces separate inputs
    if (input == "~") {
        // The user typed nothing
        return EMPTY_MOVE;
    } else if (input == "RESIGN") {
        return RESIGNED;
    } else if (input == "AIUTO") {
        return HELP_PAGE;
    } else if (input == "SUMMARY") {
        return SUMMARY;
    }
    // The move wasn't empty nor was it a command
    if (input.size() < 5) {
        return TOO_SHORT;
    }
    return VALID;
}

bool UI::dispatch_command(GameEngine& engine, std::string &command, PlayerColor currentPlayer, Move& move) {
    if (command == "AIUTO") {
        engine.execute_command(HELP_PAGE);
        move.type.moveReturn = HELP_PAGE;
        return true;
    } else if (command == "RESIGN") {
        if (currentPlayer == BIANCO) {
            move.type.moveReturn = WHITE_RESIGN;
            return true;
        } else if (currentPlayer == NERO) {
            move.type.moveReturn = BLACK_RESIGN;
            return true;
        }
    } else if (command == "SUMMARY") {
        engine.execute_command(SUMMARY);
        move.type.moveReturn = SUMMARY;
        return true;
    }
    return false;
}

MoveReturn UI::get_move(Move& move, GameEngine& engine, PlayerColor currentPlayer) {
    std::vector<std::string> input;
    std::vector<Command> commands;

    move = Move(currentPlayer);

    std::string raw_input;

    if (currentPlayer == BIANCO) {
        std::cout << "Mossa di " << PLAYER_COLOR << engine.whitePlayer.name << RESET << " > ";
    } else if (currentPlayer == NERO) {
        std::cout << "Mossa di " << PLAYER_COLOR << engine.blackPlayer.name << RESET << " > ";
    }
    getline(std::cin, raw_input);
    std::stringstream stream(raw_input);

    // Initializing the strings
    input.emplace_back("~");
    input.emplace_back("~");

    // Check input
    for (int i = 0; i < MAX_COMMANDS; i++) {
        stream >> input[i];

        std::transform(input[i].begin(), input[i].end(), input[i].begin(), ::toupper);

        if (check_input(input[i]) == VALID) {
            // If the input passed basic checks
            commands.emplace_back(input[i], engine);
        } else {
            // The input was either a command or wrong
            if (dispatch_command(engine, input[i], currentPlayer, move)) {
                // The move was a command
                return move.type.moveReturn;
            } else if (check_input(input[i]) != EMPTY_MOVE){
                // If the move was not a command, it was too short
                move.type.moveReturn = TOO_SHORT;
                return TOO_SHORT;
            }
        }
        if (commands.empty()) {
            move.type.moveReturn = NO_MOVE;
            return NO_MOVE;
        }
        // Check if the Command constructor founs anything strange
        if (commands[i].type.moveReturn != VALID && commands.empty()) {
            // If something went wrong
            if (commands[i].type.moveReturn == UNDEFINED) {
                move.type = commands[i].type;
                return move.type.moveReturn;
            }
        }
        if (commands[i].type.moveReturn == MISINPUT) {
            move.type = commands[i].type;
            return move.type.moveReturn;
        }
    }
    // If the move's type is VALID, engine will execute it, or log the error and ask for more input
    command_to_move(commands, move);
    // Check if the move's color is correct
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
            std::cout << ERROR_COLOR << "Non puoi soffiare in questo caso, scrivi aiuto per informazioni" << RESET;
            break;
        case UNDEFINED:
            std::cout << ERROR_COLOR << "Non puoi soffiare e basta, scrivi aiuto per informazioni sulla sintassi delle mosse" << RESET;
            break;
        case TOO_SHORT:
            std::cout << ERROR_COLOR << "La mossa è troppo corta, scrivi aiuto per informazioni" << RESET;
            break;
        case WRONG_OPERATOR:
            std::cout << ERROR_COLOR << "Hai sbagliato operatore, scrivi aiuto per informazioni" << RESET;
            break;
        case WRONG_COLOR:
            std::cout << ERROR_COLOR << "Stai provando a muovere pezzi del colore sbagliato, scrivi aiuto per informazioni" << RESET;
            break;
        case MISINPUT:
            std::cout << ERROR_COLOR << "Le coordinate della messa in input sono sbagliate, scrivi aiuto per informazioni" << RESET;
            break;
        case NO_MOVE:
            std::cout << ERROR_COLOR << "Non hai inserito nessuna mossa" << RESET;
            break;
    }
    std::cout << std::endl;
}
