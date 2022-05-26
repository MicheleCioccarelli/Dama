#include "fileHandler.h"

fileIssue FileHandler::create_file(const GameEngine &engine, const std::string& gameName, std::string &filePath, const int gameState) {

    if (filePath.find(gameName) == std::string::npos) {
        // The user didn't already put the game name in the path
        if (filePath.size() > gameName.size()) {
            filePath.append("/");
    }
        filePath.append(gameName);
    }

    // If the file path/name did not have the right extention, add it
    if (filePath.find(".dama") == std::string::npos) {
        filePath.append(".dama");
    }

    // You can provide the file path. If you just say the name of the game it will be created in the working directory
    std::ofstream o_file {filePath};

    if (!o_file) {
        // Something went wrong
        return OPENING_ISSUE;
    } else {
        o_file << "@BLACK:" << engine.get_player_name(NERO) << "\n";
        o_file << "@WHITE:" << engine.get_player_name(BIANCO) << "\n";
        o_file << "@NAME:" << gameName << "\n";
        o_file << "@STATE:" << gameState << "\n";
        o_file << "@MOVES:";
        // ADD ALL THE MOVES
        std::string temp {};
        int movesToPrint = engine.whitePlayer.moves.size() + engine.blackPlayer.moves.size();
        for (int i = 0; i < engine.whitePlayer.moves.size(); i++) {
            temp = "";
            temp += engine.whitePlayer.moves[i].toStringHuman();
            o_file << "[" << temp << "]";

            movesToPrint--;

            if (movesToPrint > 0) {
                o_file << ",";
            }
            temp = "";
            if (engine.blackPlayer.moves.size() > i) {
                temp += engine.blackPlayer.moves[i].toStringHuman();
                o_file << "[" << temp << "]";

                movesToPrint--;

                if (movesToPrint > 0) {
                    o_file << ",";
                }
            }
        }
    }
    // CHECK BETTER BLOW MOVES AND REPETITION

    o_file.close();
    return FILE_CLOSED_CORRECTELY;
}

void FileHandler::print_move_sequence(const GameEngine& engine) {
    std::string temp {};
    int movesToPrint = engine.whitePlayer.moves.size() + engine.blackPlayer.moves.size();
    for (int i = 0; i < engine.whitePlayer.moves.size(); i++) {
        temp = "";
        temp += engine.whitePlayer.moves[i].toStringHuman();
        std::cout << "[" << temp << "]";

        movesToPrint--;

        if (movesToPrint > 0) {
            std::cout << ",";
        }
        temp = "";
        if (engine.blackPlayer.moves.size() > i) {
            temp += engine.blackPlayer.moves[i].toStringHuman();
            std::cout << "[" << temp << "]";

            movesToPrint--;

            if (movesToPrint > 0) {
                std::cout << ",";
            }
        }
    }
    std::cout << "\n";
}

Coords FileHandler::convert_coords(const std::string &toConvert) {
    Coords converted;
    // Initialize startingColumn
    switch (toConvert[0]) {
        case 'a':
        case 'A':
            converted.column = A;
            break;
        case 'b':
        case 'B':
            converted.column = B;
            break;
        case 'C':
            converted.column = C;
            break;
        case 'd':
        case 'D':
            converted.column = D;
            break;
        case 'e':
        case 'E':
            converted.column = E;
            break;
        case 'f':
        case 'F':
            converted.column = F;
            break;
        case 'g':
        case 'G':
            converted.column = G;
            break;
        case 'h':
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

std::vector<Move> FileHandler::notation_to_moves(const std::string &notation) {
    std::vector<Move> moves;
    Move tempMove;
    std::size_t i, j, k; // Counters
    int toShift = 0; // By how much i should be shifted after a move has been processed (the move's "lenght")
    PlayerColor color = NERO;

    for (i = 0; i < notation.size(); i++) {
        if (notation.at(i) == '[') {
            if (color == BIANCO) {
                color = NERO;
            } else {
                color = BIANCO;
            }
            tempMove.playerColor = color;
            // Decode the move until you finish it
            do {
                i++;
                // Very similar code do UI::input_to_move()
                switch (notation[i + 2]) {
                    case '-':
                        tempMove.m_status = VALID;
                        tempMove.moveType = MOVE;
                        tempMove.startingCoord = convert_coords(notation.substr(i, 2));
                        tempMove.endingCoord = convert_coords(notation.substr(i + 3, 2));
                        toShift += 6;
                        break;
                    case 'x':
                        tempMove.m_status = VALID;
                        tempMove.moveType = EAT;
                        tempMove.startingCoord = convert_coords(notation.substr(i, 2));
                        // The initializer at += 3 is to skip startingCoords, which was converted above, not sure if the condition is optimal
                        for (i += 3; i <= notation.size() - 1; i += 3) {
                            // You can eat up to 3 pieces at a time
                            if (notation.at(i) == '[' || notation.at(i) == ',')
                                break;
                            // If you are blowing pieces they should not be in eatenCoords[]
//                            if (notation.at(j - 1) == '_' || notation.at(j - 1) == '*')
//                                break;
                            tempMove.eatenCoords.push_back(convert_coords(notation.substr(i, 2)));
                        }
                        tempMove.calculate_endingCoord();
                        break;
                    case '_':
                    case '*':
                        tempMove.blownCoord = convert_coords(notation.substr(i, 2));
                        tempMove.endingBlowCoord = convert_coords(notation.substr(i + 3, 2));
                        toShift += 5;
                        break;
                }
                i += toShift;
                toShift = 0;
                if (i > notation.size() - 1) {
                    break;
                }
            } while (notation.at(i) != ']' && notation.at(i) != '[' && notation.at(i) != ','); //  Spageth
                moves.push_back(tempMove);
                tempMove.clear();
        }
    }
    return moves;
}
