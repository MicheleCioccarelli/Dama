#include "square.h"

void Square::set_piece(PieceType _piece, PlayerColor _color) {
    piece = Piece(_color, _piece);
}

Square &Square::operator=(const Square &rhs)  {
    coords = rhs.coords;
    color = rhs.color;
    piece = rhs.piece;
    return *this;
}
