#include "move.h"

Move::Move(PlayerColor _color, MoveType _moveType) {
    color = _color;
    type = _moveType;
}

void Move::add_coords(const Coords &_coords) {
    coords.push_back(_coords);
}