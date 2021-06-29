#pragma once

#include "../enums/ColumnNoatiton.h"
#include "../enums/Piece.h"

class Coords {
public:
    ColumnNotation column;
    int row;

    Coords(ColumnNotation _column = Z, int _row = 0)
            : column(_column), row(_row) {}
};