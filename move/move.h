#pragma once

#include <iostream>
#include <vector>
#include "../coords/coords.h"
#include "../enums/Piece.h"
#include "../enums/MoveType.h"
#include "../enums/PlayerColor.h"

class Move {
public:
    /*
     * coords' first element is always the startingCoord of a move,
     * while the last is the ending coord.
     * Anything in between is the coords of the pieces eaten.
     */
    std::vector<Coords> coords;
    Coords blownCoord;

    PlayerColor color;
    MoveType type;

    void add_coords(const Coords & _coords);

    Move(Coords _startingCoords, Coords _endingCoords, PlayerColor _color = TRASPARENTE, MoveType _type = EAT);

    Move(PlayerColor _color, MoveType _moveType = UNINITIALIZED);
};
