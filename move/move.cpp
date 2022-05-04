#include "move.h"

Move::Move(PlayerColor color, MoveType type)
    : playerColor (color), moveType (type) {}

Move::Move() {
    playerColor = TRASPARENTE;
}

void Move::pop_coords() {
    eatenCoords.erase(eatenCoords.end() - 1);
}

std::string Move::toString() const{
    std::string ouput {};

    if (!blownCoord.is_uninitialized()) {
        ouput += blownCoord.toString();
        ouput += "*";
        ouput += endingBlowCoord.toString();
        ouput += "_";
    }

    ouput += startingCoord.toString();

    // OPERATORS
    switch (moveType) {
        case MOVE:
            ouput += "-";
            ouput += endingCoord.toString();
            break;
        case EAT:
            ouput += "x";
            for (Coords coord : eatenCoords) {
                ouput += coord.toString();
                ouput += "x";
            }
            // Delete the last excess "x"
            ouput.pop_back();
            break;
    }
    return ouput;
}

bool Move::is_misinput() const {
    if (startingCoord.is_uninitialized()) {
        return true;
    }
    for (Coords a : eatenCoords) {
        if (a.is_uninitialized()) {
            return true;
        }
    }
    return false;
}

bool Move::is_empty() const {
    if (startingCoord.is_uninitialized() && endingCoord.is_uninitialized() &&
        blownCoord.is_uninitialized() && eatenCoords.empty() && eatenPieces.empty()) {
        return true;
    }
    return false;
}

Move &Move::operator=(const Move& rhs) {
    startingCoord = rhs.startingCoord;
    endingCoord = rhs.endingCoord;
    eatenCoords = rhs.eatenCoords;
    eatenPieces = rhs.eatenPieces;
    blownCoord = rhs.blownCoord;
    playerColor = rhs.playerColor;
    moveType = rhs.moveType;
    return *this;
}

Move::Move(Coords startingCoord, Coords endingCoord, PlayerColor color, MoveType type)
        : startingCoord(startingCoord), endingCoord(endingCoord), playerColor(color), moveType(type) {}

Move::Move(Coords startingCoords, MoveType type)
        : startingCoord(startingCoords), moveType(type) {}

Move::Move(Coords startingCoord, Coords endingCoord, MoveType type, PlayerColor color)
        : startingCoord(startingCoord), endingCoord(endingCoord), playerColor(color), moveType(type) {}

Move::Move(Coords startingCoords, PlayerColor color, MoveType type)
        : startingCoord(startingCoords), playerColor(color), moveType(type) {}

void Move::convert_all() {
    startingCoord = startingCoord.convert_coords();
    if (!endingCoord.is_uninitialized()) {
        endingCoord = endingCoord.convert_coords();
    }
    for (Coords& a : eatenCoords) {
        a = a.convert_coords();
    }
    if (!blownCoord.is_uninitialized()) {
        blownCoord = blownCoord.convert_coords();
    }
}

void Move::push_coords(const Coords &_coords) {
    eatenCoords.push_back(_coords);
}

Coords Move::calculate_forward(const Coords &startingCoords, const Coords &endingCoords) {
    int verticalDistance = startingCoords.row - endingCoords.row;
    int horizontalDistance = startingCoords.column - endingCoords.column;

    return {(ColumnNotation)(endingCoords.column - horizontalDistance), endingCoords.row - verticalDistance};
}

void Move::calculate_endingCoord() {
    Coords tempForward = calculate_forward(startingCoord, eatenCoords[0]);
    for (int i = 1; i < eatenCoords.size(); i++) {
        tempForward = calculate_forward(tempForward, eatenCoords[i]);
    }
    endingCoord = tempForward;
}
