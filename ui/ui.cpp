#include "ui.h"

MoveData UI::get_move(Move& move, GameEngine& engine, const PlayerColor& currentPlayer) {
    std::string input;
    move = Move(currentPlayer);
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

    // Tells you wether a move is a command or has lenght errors
    MoveData result = check_input(input);
    // Stores any problems with the move
    MoveIssue issue = ALL_GOOD;
    // Moves with a blow in them are separeted with an _
    int _position = -1;
    // Preliminary input checking
    if (input == "~") {
        // The user typed nothing
        issue = EMPTY_MOVE;
    } else if (input.size() < 5) {
        // The move was of the wrong lenght but wasn't a command
        issue = TOO_SHORT;
    }
    if (result == VALID && issue == ALL_GOOD) {
        // Find an _ if there is any. If not position remains -1
        for (int i = 0; i < input.size(); i++) {
            if (input[i] == '_') {
                _position = i;
                break;
            }
        }
        // The move is legitimate
        move.playerColor = currentPlayer;
        if (_position == -1) {
            // There is no blow-type move
            issue = input_to_move(input, move, engine);
        } else {
            // Separate the 2 moves
            issue = input_to_move(input.substr(0, _position), move, engine);
            if (issue == ALL_GOOD) {
                input_to_move(input.substr(_position + 1, input.size() - _position), move, engine);
            }
        }
        // Now a move has been created with the input provided, if there are problems they are in issue
    } else {
        if (validate_command(input, currentPlayer, move) != INVALID) {
            // If the input was a command handle it
            handle_command(validate_command(input, currentPlayer, move), engine);
            }
        }

    if (engine.deduce_color(move) != currentPlayer) {
        issue = WRONG_COLOR;
    }
    if (issue != ALL_GOOD) {
        // There was a problem with the move
        handle_issue(issue);
        return INVALID;
    }
    return VALID;
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

MoveIssue UI::input_to_move(const std::string &input, Move &move, GameEngine& engine) {
    switch (input[2]) {
        case '-':
            if (move.type != UNINITIALIZED) {
                // The original input contained 2 eat-type moves or 2 move-type moves
                return DOUBLE_EVENT;
            }

            move.type = MOVE;
            move.startingCoord = convert_coords(input.substr(0, 2));
            move.endingCoord = convert_coords(input.substr(3, 5));
            break;
        case 'X':
            if (move.type != UNINITIALIZED) {
                return DOUBLE_EVENT;
            }

            move.type = EAT;
            move.startingCoord = convert_coords(input.substr(0, 2));
            // Add the coords which were eaten
            for (int i = 3; i <= input.size(); i += 3) {
                // You can eat up to 3 pieces at a time
                move.eatenCoords.push_back(convert_coords(input.substr(i, i + 1)));
            }
            move.calculate_endingCoord();
            break;
        case '*':
            if (!move.blownCoord.is_uninitialized()) {
                return DOUBLE_EVENT;
            }

            Move tempMove = Move(convert_coords(input.substr(0, 2)),
                                 convert_coords(input.substr(3, 5)), move.playerColor, EAT);
            MoveIssue result = engine.check_blow(tempMove);
            // engine said the move was correct
            if (result == BLOWABLE) {
                move.blownCoord = convert_coords(input.substr(3, 5));
            } else {
                return result;
            }
        }
    for (Coords currentCoord : move.eatenCoords) {
        if (currentCoord.is_uninitialized()) {
            return MISINPUT;
        }
    }
    if (move.startingCoord.is_uninitialized()) {
        return MISINPUT;
    }
    return ALL_GOOD;
}

void UI::handle_issue(MoveIssue issue) {
    UI::log_error(issue);
}

void UI::handle_command(MoveData command, const GameEngine& engine) {
    switch (command) {
        case SUMMARY:
            engine.execute_command(SUMMARY);
        case HELP_PAGE:
            engine.execute_command(HELP_PAGE);
        case WHITE_RESIGN:
            engine.execute_command(WHITE_RESIGN);
        case BLACK_RESIGN:
            engine.execute_command(BLACK_RESIGN);
        case W_DRAW_OFFER:
            engine.execute_command(W_DRAW_OFFER);
        case B_DRAW_OFFER:
            engine.execute_command(B_DRAW_OFFER);
        default:
            break;
    }
}

MoveData UI::check_input(const std::string &input) {
    if (input == "RESIGN") {
        return RESIGNED;
    } else if (input == "AIUTO") {
        return HELP_PAGE;
    } else if (input == "SUMMARY") {
        return SUMMARY;
    } else if (input == "DRAW") {
        return DRAW_OFFER;
    }
    return VALID;
}

MoveData UI::validate_command(std::string &command, PlayerColor currentPlayer, Move& move) {
    if (command == "HELP") {
        return HELP_PAGE;
    } else if (command == "RESIGN") {
        if (currentPlayer == BIANCO) {
            return WHITE_RESIGN;
        } else if (currentPlayer == NERO) {
            return BLACK_RESIGN;
        }
    } else if (command == "SUMMARY") {
        return SUMMARY;
    } else if (command == "DRAW") {
        if (currentPlayer == BIANCO) {
            return W_DRAW_OFFER;
        } else if (currentPlayer == NERO) {
            return B_DRAW_OFFER;
        }
    }
    return INVALID;
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

void UI::log_error(MoveIssue error) {
    switch (error) {
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
        case EMPTY_MOVE:
        case NO_MOVE:
            std::cout << ERROR_COLOR << "Non hai inserito nessuna mossa" << RESET;
            break;
        default:
            std::cout << HYEL << "Ho hittato un default" << RESET;
    }
    std::cout << std::endl;
}
