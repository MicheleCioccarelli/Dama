#pragma once

#include "../coords/coords.h"
#include "../enums/Piece.h"

class Move {
public:
    Coords startingCoord;
    Coords endingCoord;

    // 1 is automatically removed from the row number, so you can write it using normal notation
    Move(Coords _startingCoord, Coords _endingCoord);
};