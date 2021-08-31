#pragma once

#include "../enums/ColumnNotatiton.h"

// Chess notation
class Coords {
public:
    ColumnNotation column;
    int row;

    Coords &operator=(const Coords &rhs);

    Coords(ColumnNotation _column = Z, int _row = 9): column(_column), row(_row) {}
};