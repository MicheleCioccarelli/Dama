#include "ui.h"

void UI::command_to_move(const std::vector<Command>& commands, Move &move) {
    for (int i = 0; i < commands.size(); i++) {
        switch (commands[i].type.moveType) {
            case MOVE:
                move.type.moveType = MOVE;
                move.add_coords(commands[i].startingCoords);
                move.add_coords(commands[i].endingCoords);
                i = commands.size();
                break;
            case EAT:
                move.type.moveType = EAT;
                for (int j = 0; j < commands[i].eatenCoords.size(); j++) {
                    move.coords.push_back(commands[i].eatenCoords[j]);
                }
                i = commands.size();
                break;
            case UNINITIALIZED:
                move.type.moveType = UNINITIALIZED;
        }
        switch (commands[i].type.moveReturn) {
            case BLOWABLE:
                move.blownCoord = commands[i].blownCoords;
                move.type.moveReturn = BLOWABLE;
                break;
            case  TOO_SHORT:
                move.type.moveReturn = TOO_SHORT;
            case WRONG_OPERATOR:
                move.type.moveReturn = WRONG_OPERATOR;
        }
    }
}

MoveReturn UI::check_color(Move &move, PlayerColor currentPlayer) {
    switch (currentPlayer)  {
        case BIANCO:
            if (GameEngine::deduce_color(move) == NERO) {
                if (move.type.moveReturn != BLOWABLE) {
                    move.type.moveReturn = WRONG_COLOR;
                }
            }
            break;
        case NERO:
            if (GameEngine::deduce_color(move) == BIANCO) {
                if (move.type.moveReturn != BLOWABLE) {
                    move.type.moveReturn = WRONG_COLOR;
                }
            }
        default:
            break;
    }
}

void UI::get_move(Move& move, GameEngine& engine, PlayerColor currentPlayer) {
    std::vector<std::string> input;
    std::vector<Command> commands;

    move = Move(currentPlayer);

    std::string a;
    std::cout << "Move > ";
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
            return;
        }
        commands.emplace_back(input[i], engine);
    }

    command_to_move(commands, move);

    UI::check_color(move, currentPlayer);

    UI::log_error(move.type.moveReturn);
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
        default:
            std::cout << "Default log_error case called";
            break;
    }
    std::cout << std::endl;
}
