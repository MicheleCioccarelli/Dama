#include "move.h"

Move::Move(PlayerColor color, MoveType type)
    : m_playerColor (color), m_type (type) {}

Move::Move() {
    m_playerColor = TRASPARENTE;
}

void Move::pop_coords() {
    m_eatenCoords.erase(m_eatenCoords.end() - 1);
}

bool Move::is_misinput() const {
    if (m_startingCoord.is_uninitialized()) {
        return true;
    }
    for (Coords a : m_eatenCoords) {
        if (a.is_uninitialized()) {
            return true;
        }
    }
    return false;
}

bool Move::is_empty() const {
    if (m_startingCoord.is_uninitialized() && m_endingCoord.is_uninitialized() &&
        m_blownCoord.is_uninitialized() && m_eatenCoords.empty() && m_eatenPieces.empty()) {
        return true;
    }
    return false;
}

Move &Move::operator=(const Move& rhs) {
    m_startingCoord = rhs.m_startingCoord;
    m_endingCoord = rhs.m_endingCoord;
    m_eatenCoords = rhs.m_eatenCoords;
    m_eatenPieces = rhs.m_eatenPieces;
    m_blownCoord = rhs.m_blownCoord;
    m_playerColor = rhs.m_playerColor;
    m_type = rhs.m_type;
    return *this;
}

Move::Move(Coords startingCoord, Coords endingCoord, PlayerColor color, MoveType type)
        : m_startingCoord(startingCoord), m_endingCoord(endingCoord), m_playerColor(color), m_type(type) {}

Move::Move(Coords startingCoords, MoveType type)
        : m_startingCoord(startingCoords), m_type(type) {}

Move::Move(Coords startingCoord, Coords endingCoord, MoveType type, PlayerColor color)
        : m_startingCoord(startingCoord), m_endingCoord(endingCoord), m_playerColor(color), m_type(type) {}

Move::Move(Coords startingCoords, PlayerColor color, MoveType type)
        : m_startingCoord(startingCoords), m_playerColor(color), m_type(type) {}

void Move::convert_all() {
    m_startingCoord = m_startingCoord.convert_coords();
    if (!m_endingCoord.is_uninitialized()) {
        m_endingCoord = m_endingCoord.convert_coords();
    }
    for (Coords& a : m_eatenCoords) {
        a = a.convert_coords();
    }
}

void Move::push_coords(const Coords &_coords) {
    m_eatenCoords.push_back(_coords);
}

Coords Move::calculate_forward(const Coords &startingCoords, const Coords &endingCoords) {
    int verticalDistance = startingCoords.row - endingCoords.row;
    int horizontalDistance = startingCoords.column - endingCoords.column;

    return {(ColumnNotation)(endingCoords.column - horizontalDistance), endingCoords.row - verticalDistance};
}

void Move::calculate_endingCoord() {
    Coords tempForward = calculate_forward(m_startingCoord, m_eatenCoords[0]);
    for (int i = 1; i < m_eatenCoords.size(); i++) {
        tempForward = calculate_forward(tempForward, m_eatenCoords[i]);
    }
    m_endingCoord = tempForward;
}
