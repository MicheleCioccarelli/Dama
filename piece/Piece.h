#pragma once

#include "../enums/PlayerColor.h"
#include "../enums/PieceType.h"

class Piece {
public:
    PlayerColor color;
    PieceType type;

    Piece& operator= (const Piece& rhs);

    Piece()
        : color(TRASPARENTE), type(VUOTA) {}
    Piece(PlayerColor _color, PieceType _type)
        : color(_color), type(_type) {}
};
