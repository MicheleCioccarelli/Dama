#include "square.h"

void Square::set_piece(Piece _piece) {
    piece = _piece;
}

Square &Square::operator=(const Square &rhs)  {
    coords = rhs.coords;
    color = rhs.color;
    piece = rhs.piece;
    return *this;
}
