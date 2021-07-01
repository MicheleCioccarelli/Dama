#pragma once

#include "../coords/coords.h"
#include "../enums/Piece.h"
#include "../enums/MoveType.h"
#include "../enums/PlayerColor.h"

class Move {
public:
    Coords startingCoord;
    Coords endingCoord;
    PlayerColor color;

    MoveType type;

    // 1 is automatically removed from the row number, so you can write it using normal notation
    Move(Coords _startingCoord, Coords _endingCoord, PlayerColor _color, MoveType _moveType = MOVE);
};
