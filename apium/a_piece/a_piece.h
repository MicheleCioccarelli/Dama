#pragma once
#include "../../piece/Piece.h"
#include "../../coords/coords.h"

class A_piece : Piece {
public:
    Coords coords;
    // The piece's value, based on its position and type
    float value;
};
