#include "coords.h"

Coords &Coords::operator=(const Coords &rhs) {
    this->row = rhs.row;
    this->column = rhs.column;
    return *this;
}
