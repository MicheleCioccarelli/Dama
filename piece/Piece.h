#pragma once

#include "../enums/PlayerColor.h"
#include "../enums/PieceType.h"

class Piece {
public:
    PlayerColor m_color;
    PieceType m_type;

    Piece& operator= (const Piece& rhs);

    Piece()
        : m_color(TRASPARENTE), m_type(VUOTA) {}
    Piece(PlayerColor color, PieceType type)
        : m_color(color), m_type(type) {}
};
