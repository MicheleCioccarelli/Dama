#include "Piece.h"

Piece& Piece::operator=(const Piece &rhs) {
    color = rhs.color;
    type = rhs.type;
    return *this;
}