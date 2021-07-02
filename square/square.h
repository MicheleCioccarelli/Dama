#pragma once

#include <iostream>

#include "../coords/coords.h"
#include "../enums/ColumnNotatiton.h"
#include "../enums/SquareColor.h"
#include "../enums/Piece.h"

#define columns 8
#define rows 8

class Square {
public:
    Coords coords;
    SquareColor color;
    Piece piece;

    void set_piece(Piece _piece);

    Square &operator=(const Square &rhs);

    Square(Coords _coords, SquareColor _color, Piece _piece = VUOTA)
            : coords(_coords), color(_color), piece(_piece) {}
};
