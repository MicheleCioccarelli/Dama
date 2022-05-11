#include "Piece.h"

Piece& Piece::operator=(const Piece &rhs) {
    color = rhs.color;
    type = rhs.type;
    return *this;
}

bool Piece::operator==(const Piece &rhs) {
    if (color == rhs.color && type == rhs.type) {
        return true;
    }
    return false;
}

bool Piece::operator!=(const Piece &rhs) {
    if (*this == rhs) {
        return false;
    }
    return true;
}