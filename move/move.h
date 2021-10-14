#pragma once

#include <iostream>
#include <vector>
#include "../coords/coords.h"
#include "../piece/Piece.h"
#include "../movecase/movecase.h"

class Move {
public:
    /*
     * coords' first element is always the startingCoord of a move,
     * while the last is the ending coord.
     * Anything in between is the coords of the pieces eaten.
     */

    // This operator is only used when rhs is an lvalue
    Move& operator= (Move rhs);

    std::vector<Coords> coords;
    Coords blownCoord;

    PlayerColor playerColor;
    MoveCase type;

    void add_coords(const Coords & _coords);

    Move();

    Move(Coords _startingCoords, Coords _endingCoords, PlayerColor _color = TRASPARENTE, MoveType _type = EAT);

    Move(PlayerColor _color, MoveType _moveType = UNINITIALIZED);
};
