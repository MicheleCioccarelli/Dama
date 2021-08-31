#include "coords.h"

Coords &Coords::operator=(const Coords &rhs) {
    this->row = rhs.row;
    this->column = rhs.column;
    return *this;
}

bool Coords::operator==(const Coords &rhs) const {
    if (this->column == rhs.column && this->row == rhs.row) {
        return true;
    } else {
        return false;
    }
}

bool Coords::operator!=(const Coords &rhs) const {
    if (this->column == rhs.column && this->row == rhs.row) {
        return false;
    } else {
        return true;
    }
}
