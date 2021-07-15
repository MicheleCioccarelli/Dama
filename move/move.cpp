#include "move.h"

Move::Move(PlayerColor _color, MoveType _moveType)
    :     color (_color), type (_moveType) {}

Move::Move(Coords _startingCoords, Coords _endingCoords, PlayerColor _color, MoveType _type) {
    this->add_coords(_startingCoords);
    this->add_coords(_endingCoords);
    color = _color;
    type = _type;
}

void Move::add_coords(const Coords &_coords) {
    coords.push_back(_coords);
}