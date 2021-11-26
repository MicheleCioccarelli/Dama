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

    Square(Coords _coords, SquareColor _color, PieceType _pieceType = VUOTA, PlayerColor _pieceColor = TRASPARENTE)
    : coords(_coords), color(_color), piece(_pieceColor, _pieceType) {}
};
