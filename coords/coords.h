#pragma once

#include "../enums/ColumnNotatiton.h"
#include <string>

// Chess notation
class Coords {
public:
    ColumnNotation column;
    int row;

    // Adds 1 to the row number
    std::string toStringHuman() const;
    //Doesn't add one to the line number
    std::string toStringMatrix() const;

    Coords &operator=(const Coords &rhs);

    bool operator==(const Coords &rhs) const;
    bool operator!=(const Coords &rhs) const;

    // Convert coords from human notation to matrix notation
    Coords convert_coords() const;

    // Reset the variable
    void clear();

    bool is_uninitialized() const;

    Coords(ColumnNotation _column = Z, int _row = 9)
    : column(_column), row(_row) {}
};