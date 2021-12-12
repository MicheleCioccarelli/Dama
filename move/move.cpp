#include "move.h"

Move::Move(PlayerColor _color, MoveType _type)
    : playerColor (_color), type (_type) {}

Move::Move() {
    playerColor = TRASPARENTE;
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

Move::Move(Coords _startingCoord, Coords _endingCoord, PlayerColor _color, MoveType _type)
            : startingCoord(_startingCoord), endingCoord(_endingCoord), playerColor(_color), type(_type) {}

void Move::add_coords(const Coords &_coords) {
    eatenCoords.push_back(_coords);
}