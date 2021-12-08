#include "ui.h"

MoveReturn UI::get_move(Move& move, GameEngine& engine, const PlayerColor& currentPlayer) {
    std::string input;

    move = Move(currentPlayer);
    // May be useless
    std::string raw_input;

    if (currentPlayer == BIANCO) {
        std::cout << "Mossa di " << PLAYER_COLOR << engine.whitePlayer.name << RESET << " > ";
    } else if (currentPlayer == NERO) {
        std::cout << "Mossa di " << PLAYER_COLOR << engine.blackPlayer.name << RESET << " > ";
    }
    getline(std::cin, raw_input);
    std::stringstream stream(raw_input);

    // Initializing the string
    input = ("~");

    stream >> input;

    std::transform(input.begin(), input.end(), input.begin(), ::toupper);
    move.type.moveType = UNINITIALIZED;
    move.type.moveReturn = LENGHT_ERROR;

    MoveReturn result = check_input(input);
    // Moves with a blow in them are separeted with an _
    int _position = -1;
    switch (result) {
        case VALID:
            for (int i = 0; i < input.size(); i++) {
                if (input[i] == '_') {
                    _position = i;
                    break;
                }
            }
            if (_position == -1) {
                input_to_move(input, move);
            } else {
                // Separate the 2 moves
                // Potential segfault
                input_to_move(input.substr(0, _position), move);
                input_to_move(input.substr(_position + 1, input.size() - _position), move);
            }
            // Now a move has been created with the input provided, if there are problems they are in move.type.moveReturn
            break;
        case EMPTY_MOVE:
            UI::log_error(EMPTY_MOVE);
            return EMPTY_MOVE;
        case TOO_SHORT:
            UI::log_error(TOO_SHORT);
            return TOO_SHORT;
        default:
        if (validate_command(input, currentPlayer, move)) {
            // If the input was a command handle it
            switch (move.type.moveReturn) {
                case SUMMARY:
                    engine.execute_command(SUMMARY);
                    return SUMMARY;
                case HELP_PAGE:
                    engine.execute_command(HELP_PAGE);
                    return HELP_PAGE;
                case WHITE_RESIGN:
                    engine.execute_command(WHITE_RESIGN);
                    return WHITE_RESIGN;
                case BLACK_RESIGN:
                    engine.execute_command(BLACK_RESIGN);
                case W_DRAW_OFFER:
                    engine.execute_command(W_DRAW_OFFER);
                    return W_DRAW_OFFER;
                case B_DRAW_OFFER:
                    engine.execute_command(B_DRAW_OFFER);
                    return B_DRAW_OFFER;
                default:
                    break;
            }
        }
    }
    switch (move.type.moveReturn) {

        case MISINPUT:
            return MISINPUT;
        case NO_MOVE:
            return NO_MOVE;
    }
    // Check if the move's color is correct
    UI::check_color(move, currentPlayer, engine);

    return move.type.moveReturn;
}

void UI::init(GameEngine &engine) {
    std::string playerName;

    std::cout << MAG << "Benvenuto nella dama" << RESET << std::endl << std::endl;

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
//
//void UI::command_to_move(const std::vector<Command>& commands, Move &move) {
//    for (int i = 0; i < commands.size(); i++) {
//        switch (commands[i].type.moveType) {
//            case MOVE:
//                move.type.moveType = MOVE;
//                if (move.type.moveReturn != BLOWABLE) {
//                    move.type.moveReturn = commands[i].type.moveReturn;
//                }
//                move.add_coords(commands[i].startingCoords);
//                move.add_coords(commands[i].endingCoords);
//                i = commands.size();
//                break;
//            case EAT:
//                move.type.moveType = EAT;
//                if (move.type.moveReturn != BLOWABLE) {
//                    move.type.moveReturn = commands[i].type.moveReturn;
//                }
//                for (const auto & eatenCoord : commands[i].eatenCoords) {
//                    move.coords.push_back(eatenCoord);
//                }
//                i = commands.size();
//                break;
//        }
//        switch (commands[i].type.moveReturn) {
//            case BLOWABLE:
//                move.blownCoord = commands[i].blownCoords;
//                move.type.moveReturn = BLOWABLE;
//                break;
//            case TOO_SHORT:
//                move.type.moveReturn = TOO_SHORT;
//            case WRONG_OPERATOR:
//                move.type.moveReturn = WRONG_OPERATOR;
//        }
//    }
//}

MoveReturn UI::validate_input(const Coords &coords) {
    if (coords.column == Z) {
        return MISINPUT;
    } else if (coords.row < 0 || coords.row > 8) {
        return MISINPUT;
    } else {
        return VALID;
    }
}

void UI::input_to_move(const std::string &input, Move &move) {
    switch (input[2]) {
        case '-':
            if (move.type.moveType != UNINITIALIZED) {
                move.type.moveReturn =  DOUBLE_EVENT;
                return;
            }

            move.type.moveType = MOVE;
            move.coords.push_back(convert_coords(input.substr(0, 2)));
            move.coords.push_back(convert_coords(input.substr(3, 5)));
            break;
        case 'X':
            if (move.type.moveType != UNINITIALIZED) {
                move.type.moveReturn =  DOUBLE_EVENT;
                return;
            }

            move.type.moveType = EAT;
            move.coords[0] = convert_coords(input.substr(0, 2));
            for (int i = 3, j = 1; i <= input.size(); i += 3, j++) {
                // You can eat up to 3 pieces at a time
                move.coords[j] = convert_coords(input.substr(i, i + 1));
            }
            break;
        default:
            break;
        }
    move.type.moveReturn = VALID;
    for (Coords currentCoord : move.coords) {
        if (currentCoord.is_uninitialized()) {
            move.type.moveReturn = MISINPUT;
        }
    }
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
    if (input == "~") {
        // The user typed nothing
        return EMPTY_MOVE;
    } else if (input == "RESIGN") {
        return RESIGNED;
    } else if (input == "AIUTO") {
        return HELP_PAGE;
    } else if (input == "SUMMARY") {
        return SUMMARY;
    } else if (input == "DRAW") {
        return DRAW_OFFER;
    }
    if (input.size() < 5) {
        // The move was of the wrong lenght but wasn't a command
        return TOO_SHORT;
    }
    return VALID;
}

bool UI::validate_command(std::string &command, PlayerColor currentPlayer, Move& move) {
    if (command == "HELP") {
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
        move.type.moveReturn = SUMMARY;
        return true;
    } else if (command == "DRAW") {
        if (currentPlayer == BIANCO) {
            move.type.moveReturn = W_DRAW_OFFER;
            return true;
        } else if (currentPlayer == NERO) {
            move.type.moveReturn = B_DRAW_OFFER;
            return true;
        }
    }
    return false;
}

Coords UI::convert_coords(const std::string& toConvert) {
    Coords converted;
    // Initialize startingColumn
    switch (toConvert[0]) {
        case 'A':
            converted.column = A;
            break;
        case 'B':
            converted.column = B;
            break;
        case 'C':
            converted.column = C;
            break;
        case 'D':
            converted.column = D;
            break;
        case 'E':
            converted.column = E;
            break;
        case 'F':
            converted.column = F;
            break;
        case 'G':
            converted.column = G;
            break;
        case 'H':
            converted.column = H;
            break;
        default:
            converted.column = Z;
    }
    // Initialize startingRow
    if (toConvert[1] - 48 < 0 || toConvert[1] > 57) {
        converted.row = 9;
    } else {
        converted.row = (int) toConvert[1] - 48;
    }
    return converted;
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
        case EMPTY_MOVE:
            std::cout << ERROR_COLOR << "Non hai scritto niente, scrivi aiuto per informazioni" << RESET;
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
