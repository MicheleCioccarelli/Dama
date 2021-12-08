#include "coords.h"

Coords Coords::convert_coords() const {
    return {column, row - 1};
}

Coords &Coords::operator=(const Coords &rhs) = default;

bool Coords::is_uninitialized() {
    if (row == 9 || column == Z) {
        return true;
    }
    return false;
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
