#pragma once

#include <iostream>

#include "../coords/coords.h"
#include "../enums/ColumnNotatiton.h"
#include "../enums/SquareColor.h"
#include "../piece/Piece.h"

class Square {
public:
    Coords coords;
    SquareColor color;
    Piece piece;

    void set_piece(PieceType _piece, PlayerColor _color);

    Square &operator=(const Square &rhs);

    Square() = default;

    Square(Coords Coords, SquareColor Color, PieceType pieceType = VUOTA, PlayerColor pieceColor = TRASPARENTE)
    : coords(Coords), color(Color), piece(pieceColor, pieceType) {}
};
