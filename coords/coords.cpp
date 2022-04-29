#include "coords.h"

Coords Coords::convert_coords() const {
    return {column, row - 1};
}

void Coords::toString(std::string& whereToStoreTheString) {
    // FIRST COORD
    switch (column) {
        case A :
            whereToStoreTheString += "A";
            break;
        case B :
            whereToStoreTheString += "B";
            break;
        case C :
            whereToStoreTheString += "C";
            break;
        case D :
            whereToStoreTheString += "D";
            break;
        case E :
            whereToStoreTheString += "E";
            break;
        case F :
            whereToStoreTheString += "F";
            break;
        case G :
            whereToStoreTheString += "G";
            break;
        case H :
            whereToStoreTheString += "H";
            break;
        default:
            whereToStoreTheString += "?";
    }
    whereToStoreTheString += std::to_string(row);
}

Coords &Coords::operator=(const Coords &rhs) {
    column = rhs.column;
    row = rhs.row;
}

bool Coords::is_uninitialized() const {
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
