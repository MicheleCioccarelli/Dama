#include "command.h"

void Command::convert_coords(const std::string& toConvert, Coords& converted) {
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
}

Command::Command(std::string& input, GameEngine& engine) {
    if (input == "~") {
         type = UNINITIALIZED;
    } else {
        Coords eatenCoord;
        switch (input[2]) {
            case '-':
                type.moveType = MOVE;
                convert_coords(input.substr(0, 2), startingCoords);
                convert_coords(input.substr(3, 5), endingCoords);
                type.moveReturn = VALID;
                break;
            case '*':
                convert_coords(input.substr(0, 2), startingCoords);
                convert_coords(input.substr(3, 5), endingCoords);
                if (engine.check_blow(startingCoords, endingCoords) != BLOWABLE) {
                    // If the move is invalid, uninitialize the coords;
                    startingCoords = Coords();
                    endingCoords = Coords();
                } else {
                    // If the move can be blown
                    type.moveReturn = BLOWABLE;
                    convert_coords(input.substr(0, 2), blownCoords);
                    type.moveReturn = VALID;
                }
                break;
            case 'x':
                type.moveType = EAT;
                convert_coords(input.substr(0, 2), startingCoords);
                eatenCoords.push_back(startingCoords);
                for (int i = 3; i <= input.size(); i += 3) {
                    convert_coords(input.substr(i, i + 1), eatenCoord);
                    eatenCoords.push_back(eatenCoord);
                }
                endingCoords = Coords();
                startingCoords = Coords();
                type.moveReturn = VALID;
                break;
            default:
                type = MoveCase(UNINITIALIZED, WRONG_OPERATOR);
                break;
        }
    }
}