#include "coords.h"

Coords Coords::convert_coords() const {
    return {column, row - 1};
}

std::string Coords::toString() const{
    std::string output {};

    // FIRST COORD
    switch (column) {
        case A :
            output += "A";
            break;
        case B :
            output += "B";
            break;
        case C :
            output += "C";
            break;
        case D :
            output += "D";
            break;
        case E :
            output += "E";
            break;
        case F :
            output += "F";
            break;
        case G :
            output += "G";
            break;
        case H :
            output += "H";
            break;
        default:
            output += "?";
    }
    output += std::to_string(row + 1);
    return output;
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
