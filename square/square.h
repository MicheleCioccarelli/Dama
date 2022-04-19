#pragma once

#include <iostream>

#include "../coords/coords.h"
#include "../enums/ColumnNotatiton.h"
#include "../enums/SquareColor.h"
#include "../piece/Piece.h"

class Square {
public:
    Coords m_coords;
    SquareColor m_color;
    Piece m_piece;

    void set_piece(PieceType piece, PlayerColor color);

    Square &operator=(const Square &rhs);

    Square() = default;

    Square(Coords coords, SquareColor color, PieceType pieceType = VUOTA, PlayerColor pieceColor = TRASPARENTE)
    : m_coords(coords), m_color(color), m_piece(pieceColor, pieceType) {}
};
