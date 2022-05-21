#include "fileHandler.h"

fileIssue FileHandler::create_file(const GameEngine &engine, const std::string& gameName, std::string &filePath, const int gameState) {

    if (filePath.find(gameName) == std::string::npos) {
        // The user didn't already put the game name in the path
        filePath.append("/");
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
        o_file << "@BLACK:" << engine.get_player_name(BIANCO) << "\n";
        o_file << "@NAME:" << gameName << "\n";
        o_file << "@STATE:" << gameState << "\n";
        o_file << "@MOVES:";
        // ADD ALL THE MOVES
        std::string temp {};
        int movesToPrint = engine.whitePlayer.moves.size() + engine.blackPlayer.moves.size();
        for (int i = 0; i < engine.whitePlayer.moves.size(); i++) {
            temp = "";
            temp += engine.whitePlayer.moves[i].toString();
            o_file << "[" << temp << "]";

            movesToPrint--;

            if (movesToPrint > 0) {
                o_file << ",";
            }
            temp = "";
            if (engine.blackPlayer.moves.size() > i) {
                temp += engine.blackPlayer.moves[i].toString();
                o_file << "[" << temp << "]";

                movesToPrint--;

                if (movesToPrint > 0) {
                    std::cout << ",";
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
        temp += engine.whitePlayer.moves[i].toString();
        std::cout << "[" << temp << "]";

        movesToPrint--;

        if (movesToPrint > 0) {
            std::cout << ",";
        }
        temp = "";
        if (engine.blackPlayer.moves.size() > i) {
            temp += engine.blackPlayer.moves[i].toString();
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
    int _position = -1; // The position of an "_" in a move
    int toShift = 0; // By how much i should be shifted after a move has been processed (the move's "lenght")

    for (i = 0; i < notation.size(); i++) {
        if (notation.at(i) == '[') {
            // Decode the move until you finish it
            do {
                i++;
                // Very similar code do UI::input_to_move()
                switch (notation[i + 2]) {
                    case '-':
                        tempMove.m_status = VALID;
                        tempMove.moveType = MOVE;
                        tempMove.startingCoord = convert_coords(notation.substr(i, i + 2));
                        tempMove.endingCoord = convert_coords(notation.substr(i + 3, i + 5));
                        toShift += 5;
                        break;
                    case 'x':
                        tempMove.m_status = VALID;
                        tempMove.moveType = EAT;
                        tempMove.startingCoord = convert_coords(notation.substr(i, i + 2));
                        // Add the coords which were eaten
                        for (j = i + 3; j <= notation.size() - 1; j += 3) {
                            // You can eat up to 3 pieces at a time
                            if (notation.at(j) == '[')
                                break;
                            // If you are blowing pieces they should not be in eatenCoords[]
                            if (notation.at(j - 1) == '_' || notation.at(j - 1) == '*')
                                break;
                            tempMove.eatenCoords.push_back(convert_coords(notation.substr(j, j + 1)));
                        }

                        tempMove.calculate_endingCoord();
                        toShift += 2 + tempMove.eatenCoords.size() * 2 + tempMove.eatenCoords.size();
                        break;
                    case '_':
                    case '*':
                        tempMove.blownCoord = convert_coords(notation.substr(i, i + 2));
                        tempMove.endingBlowCoord = convert_coords(notation.substr(i + 3, i + 5));
                        toShift += 5;
                        break;
                }
                i += toShift;
                toShift = 0;
            } while (notation.at(i) != ']');

            if (notation.at(i) == ']') {
                moves.push_back(tempMove);
                tempMove = Move();
            }
        }
    }
    return moves;
}
