#include "ui.h"

MoveData UI::get_move(Move& move, GameEngine& engine, const PlayerColor& currentPlayer) {
    std::string input;
    move = Move(currentPlayer);
    std::string raw_input;

    if (currentPlayer == BIANCO) {
        std::cout << "Mossa di " << PLAYER_COLOR << engine.m_whitePlayer.m_name << RESET << " > ";
    } else if (currentPlayer == NERO) {
        std::cout << "Mossa di " << PLAYER_COLOR << engine.m_blackPlayer.m_name << RESET << " > ";
    }

    getline(std::cin, raw_input);
    std::stringstream stream(raw_input);

    // Initializing the string
    input = ("~");
    stream >> input;
    std::transform(input.begin(), input.end(), input.begin(), ::toupper);

//    // Tells you wether a move is a command or has lenght errors
//    MoveData result = check_input(input);
    if (validate_command(input, currentPlayer) != INVALID) {
        // If the input was a command return it. GameHandler will take care of it
        return validate_command(input, currentPlayer);
    }

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
    if (issue == ALL_GOOD) {
        // Find an _ if there is any. If not position remains -1
        for (int i = 0; i < input.size(); i++) {
            if (input[i] == '_') {
                _position = i;
                break;
            }
        }
        // The move is legitimate
        move.m_playerColor = currentPlayer;
        if (_position == -1) {
            // There is no blow-type move
            issue = input_to_move(input, move, engine);
        } else {
            // Separate the 2 moves
            issue = input_to_move(input.substr(0, _position), move, engine);
            if (issue == ALL_GOOD) {
                issue = input_to_move(input.substr(_position + 1, input.size() - _position), move, engine);
            }
        }
        // Now a move has been created with the input provided, if there are problems they are in issue
    }

    if (issue != ALL_GOOD) {
        // There was a syntactic error
        handle_issue(issue);
        return INVALID;
    }
    return VALID;
}

void UI::init(GameEngine &engine) {
    std::string playerName;

    std::cout << MAG << "Benvenuto nella dama (v 0.3)" << RESET << std::endl << std::endl;

    std::cout << "Chi gioca bianco?" << std::endl;
    getline(std::cin, playerName);
    if (playerName.size() <= 30) {
        engine.m_whitePlayer.m_name = playerName;
        engine.m_whitePlayer.m_color = BIANCO;
    } else {
        engine.m_whitePlayer.m_name = "Nope";
        engine.m_whitePlayer.m_color = BIANCO;
    }

    std::cout << "Chi gioca nero?" << std::endl;
    getline(std::cin, playerName);
    if (playerName.size() <= 30) {
        engine.m_blackPlayer.m_name = playerName;
        engine.m_blackPlayer.m_color = NERO;
    } else {
        engine.m_blackPlayer.m_name = "Nope";
        engine.m_blackPlayer.m_color = NERO;
    }
}

MoveIssue UI::input_to_move(const std::string &input, Move &move, GameEngine& engine) {
    MoveIssue result;
    Move tempMove;
    switch (input[2]) {
        case '-':
            if (move.m_type != UNINITIALIZED) {
                // The original input contained 2 eat-type moves or 2 move-type moves
                return DOUBLE_EVENT;
            }

            move.m_type = MOVE;
            move.m_startingCoord = convert_coords(input.substr(0, 2));
            move.m_endingCoord = convert_coords(input.substr(3, 5));
            break;
        case 'X':
            if (move.m_type != UNINITIALIZED) {
                return DOUBLE_EVENT;
            }

            move.m_type = EAT;
            move.m_startingCoord = convert_coords(input.substr(0, 2));
            // Add the coords which were eaten
            for (int i = 3; i <= input.size(); i += 3) {
                // You can eat up to 3 pieces at a time
                move.m_eatenCoords.push_back(convert_coords(input.substr(i, i + 1)));
            }
            move.calculate_endingCoord();
            break;
        case '*':
            if (!move.m_blownCoord.is_uninitialized()) {
                return DOUBLE_EVENT;
            }

            tempMove = Move(convert_coords(input.substr(0, 2)), move.m_playerColor, EAT);
            tempMove.push_coords(convert_coords(input.substr(3, 5)));
            result = engine.check_blow(tempMove);
            // engine said the move was correct
            if (result == BLOWABLE) {
                move.m_blownCoord = tempMove.m_startingCoord;
            } else {
                result = ROCK_SOLID;
                return result;
            }
            break;
        default:
            return MISINPUT;
        }
    for (Coords currentCoord : move.m_eatenCoords) {
        if (currentCoord.is_uninitialized()) {
            return MISINPUT;
        }
    }
    if (move.m_startingCoord.is_uninitialized() && move.m_type != UNINITIALIZED) {
        return MISINPUT;
    }
    return ALL_GOOD;
}

void UI::handle_issue(MoveIssue issue) {
    UI::log_error(issue);
}

MoveData UI::validate_command(std::string &input, PlayerColor currentPlayer) {
    if (input == "AIUTO") {
        return HELP_PAGE;
    } else if (input == "RESIGN") {
        if (currentPlayer == BIANCO) {
            return WHITE_RESIGN;
        } else if (currentPlayer == NERO) {
            return BLACK_RESIGN;
        }
    } else if (input == "SUMMARY") {
        return SUMMARY;
    } else if (input == "DRAW") {
        if (currentPlayer == BIANCO) {
            return W_DRAW_OFFER;
        } else if (currentPlayer == NERO) {
            return B_DRAW_OFFER;
        }
    } else if (input == "QUIT") {
        return QUIT;
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
            std::cout << ERROR_COLOR << "Le coordinate della mossa in input sono sbagliate, scrivi aiuto per informazioni" << RESET;
            break;
        case EMPTY_MOVE:
        case NO_MOVE:
            std::cout << ERROR_COLOR << "Non hai inserito nessuna mossa, scrivi aiuto per informazioni" << RESET;
            break;
        case WRONG_LAST_MOVE:
            std::cout << ERROR_COLOR << "L'ultima mossa dell'altro giocatore non permette di soffiare, scrivi aiuto per informazioni" << RESET;
            break;
        case NOT_ENOUGH_MOVES:
            // Only appears if you blow on the first turn
            std::cout << ERROR_COLOR << "Calmaaaaaaaa" << RESET;
            break;
        case DOUBLE_EVENT:
            std::cout << ERROR_COLOR << "Hai provato a muovere/mangiare 2 volte nella stessa mossa, scrivi aiuto per informazioni" << RESET;
            break;
    }
    std::cout << "\n";
}
