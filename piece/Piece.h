#pragma once

#include "../enums/PlayerColor.h"
#include "../enums/PieceType.h"

class Piece {
public:
    PlayerColor color;
    PieceType type;

    Piece& operator= (const Piece& rhs);
    bool operator==(const Piece& rhs);
    bool operator!=(const Piece& rhs);

    Piece()
        : color(TRASPARENTE), type(VUOTA) {}
    Piece(PlayerColor color, PieceType type)
        : color(color), type(type) {}
};
