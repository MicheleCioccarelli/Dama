#include "ui.h"

void UI::init(GameEngine &engine) {
    std::string playerName;
    std::cout << "Benvenuto nella dama [wip]" << std::endl;

    std::cout << "Chi gioca bianco?" << std::endl;
    getline(std::cin, playerName);
    if (playerName.size() <= 30) {
        engine.whitePlayer.name = playerName;
    }

    std::cout << "Chi gioca nero?" << std::endl;
    getline(std::cin, playerName);
    if (playerName.size() <= 30) {
        engine.blackPlayer.name = playerName;
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
        std::cout << "Mossa di " << engine.whitePlayer.name << " > ";
    } else if (currentPlayer == NERO) {
        std::cout << "Mossa di " << engine.blackPlayer.name << " > ";
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
            UI::log_error(move.type.moveReturn);
            return TOO_SHORT;
        }
        std::transform(input[i].begin(), input[i].end(), input[i].begin(), ::toupper);
        commands.emplace_back(input[i], engine);
    }

    // Proceed with the program only if the coordinates are right, this should avoid segfault
    if (validate_command(commands) == VALID) {
        command_to_move(commands, move);
    } else {
        log_error(MISINPUT);
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
            std::cout << "Ti stai muovendo in una casella bianca";
            break;
        case TOO_FAR:
            std::cout << "Non puoi muoverti di piu di una casella alla volta";
            break;
        case BEHIND:
            std::cout << "La damina non puomuoversi indietro";
            break;
        case POPULATED:
            std::cout << "La casella che vuoi occupare è occupata";
            break;
        case EMPTY_START:
            std::cout << "La casella di partenza è vuota";
            break;
        case EMPTY_TARGET:
            std::cout << "Non puoi mangiare il niente";
            break;
        case CANNIBALISM:
            std::cout << "Non commettere cannibalismo";
            break;
        case TOO_BIG:
            std::cout << "La tua damina è troppo ambiziosa";
            break;
        case FRIENDLY_FIRE:
            std::cout << "Fuoco amico non attivo";
            break;
        case INVALID:
            std::cout << "Qualcosa è andato storto";
            break;
        case OUT_OF_BOUNDS:
            std::cout << "Andresti fuori dalla scacchiera";
            break;
        case ROCK_SOLID:
            std::cout << "Non puoi soffiarla";
            break;
        case UNDEFINED:
            std::cout << "Qualcosa è andato molto storto";
            break;
        case TOO_SHORT:
            std::cout << "La mossa è troppo corta";
            break;
        case WRONG_OPERATOR:
            std::cout << "Hai sbagliato operatore, prova a scrivere help";
            break;
        case WRONG_COLOR:
            std::cout << "Stai provando a muovere pezzi del colore sbagliato";
            break;
        case MISINPUT:
            std::cout << "La mossa messa in input è sbagliata";
            break;
        default:
            std::cout << "Default log_error case called";
            break;
    }
    std::cout << std::endl;
}
