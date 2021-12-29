#include "move.h"

Move::Move(PlayerColor _color, MoveType _type)
    : playerColor (_color), type (_type) {}

Move::Move() {
    playerColor = TRASPARENTE;
}

bool Move::is_misinput() {
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

bool Move::is_empty() {
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
    type = rhs.type;
    return *this;
}

Move::Move(Coords _startingCoords, MoveType _type)
        : startingCoord(_startingCoords), type(_type) {}

Move::Move(Coords _startingCoord, Coords _endingCoord, PlayerColor _color, MoveType _type)
        : startingCoord(_startingCoord), endingCoord(_endingCoord), playerColor(_color), type(_type) {}

void Move::convert_all() {
    startingCoord = startingCoord.convert_coords();
    if (!endingCoord.is_uninitialized()) {
        endingCoord = endingCoord.convert_coords();
    }
    for (Coords& a : eatenCoords) {
        a = a.convert_coords();
    }
}

void Move::add_coords(const Coords &_coords) {
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