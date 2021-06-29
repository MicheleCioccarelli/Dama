#pragma once

#include <iostream>

#include "../engine/engine.h"
#include "../enums/ColumnNoatiton.h"
#include "../enums/SquareColor.h"
#include "../enums/Piece.h"

#define columns 8
#define rows 8

class Coords {
public:
    ColumnNotation column;
    int row;

    Coords(ColumnNotation _column = Z, int _row = 0)
            : column(_column), row(_row) {}
};

class Square {
public:
    const Coords coords;
    const SquareColor color;
    Piece piece;

    void set_piece(Piece _piece);

    Square(Coords _coords, SquareColor _color, Piece _piece = VUOTA)
            : coords(_coords), color(_color), piece(_piece) {}
};
