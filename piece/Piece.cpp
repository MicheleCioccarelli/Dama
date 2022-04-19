#include "Piece.h"

Piece& Piece::operator=(const Piece &rhs) {
    m_color = rhs.m_color;
    m_type = rhs.m_type;
    return *this;
}