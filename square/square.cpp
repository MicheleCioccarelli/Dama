#include "square.h"

void Square::set_piece(PieceType piece, PlayerColor color) {
    m_piece = Piece(color, piece);
}

Square &Square::operator=(const Square &rhs)  {
    m_coords = rhs.m_coords;
    m_color = rhs.m_color;
    m_piece = rhs.m_piece;
    return *this;
}
