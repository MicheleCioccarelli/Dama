#pragma once

#include "../coords/coords.h"
#include "../enums/Piece.h"
#include "../enums/MoveType.h"

class Move {
public:
    Coords startingCoord;
    Coords endingCoord;
    MoveType moveType;

    // 1 is automatically removed from the row number, so you can write it using normal notation
    Move(Coords _startingCoord, Coords _endingCoord, MoveType _moveType = MOVE);
};
