#pragma once

#include <iostream>
#include <vector>
#include "../coords/coords.h"
#include "../enums/Piece.h"
#include "../enums/MoveType.h"
#include "../enums/PlayerColor.h"

class Move {
public:
    Coords startingCoord;
    Coords endingCoord;

    std::vector<Coords> piecesEaten;
    Coords blownCoord;

    PlayerColor color;
    MoveType type;

    void fill_eat(const std::vector<Coords>& coords);

    // 1 is automatically removed from the row number, so you can write it using normal notation
    Move(Coords _startingCoord, Coords _endingCoord, PlayerColor _color, MoveType _moveType);
};
