#include "move.h"

Move::Move(PlayerColor _color, MoveType _moveType)
    : playerColor (_color), type (_moveType) {}

Move::Move() {
    playerColor = TRASPARENTE;
}

Move &Move::operator=(Move rhs) {
    coords = rhs.coords;
    blownCoord = rhs.blownCoord;
    playerColor = rhs.playerColor;
    type = rhs.type;
    return *this;
}

Move::Move(Coords _startingCoords, Coords _endingCoords, MoveType _type) {
    this->add_coords(_startingCoords);
    this->add_coords(_endingCoords);
    playerColor = TRASPARENTE;
    type = _type;
}

Move::Move(Coords _startingCoords, Coords _endingCoords, PlayerColor _color, MoveType _type) {
    this->add_coords(_startingCoords);
    this->add_coords(_endingCoords);
    playerColor = _color;
    type = _type;
}

void Move::add_coords(const Coords &_coords) {
    coords.push_back(_coords);
}