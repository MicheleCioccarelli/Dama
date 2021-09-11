#pragma once

#include "../enums/ColumnNotatiton.h"

// Chess notation
class Coords {
public:
    ColumnNotation column;
    int row;

    Coords &operator=(const Coords &rhs);
    bool operator==(const Coords &rhs) const;
    bool operator!=(const Coords &rhs) const;

    // Convert coords from human notation to matrix notation
    Coords convert_coords();

    Coords(ColumnNotation _column = Z, int _row = 9)
    : column(_column), row(_row) {}
};