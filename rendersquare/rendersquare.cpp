#include "rendersquare.h"

RenderSquare::RenderSquare(Coords coords) {
    m_coords = coords;
    m_metaColor = BOARD_COLOR;
    // Format special squares depending on their position in the board
    switch (m_coords.row) {
        case 0:
            if (m_coords.column == 0) {
                m_bottomLeftCorner = BOTTOM_LEFT;
                m_bottomRightCorner = DOWN_CHAIN;
                m_topLeftCorner = LEFT_BORDER;
                m_topRightCorner = LINK;
            } else if (m_coords.column == COLUMNS - 1) {
                m_bottomRightCorner = BOTTOM_RIGHT;
                m_bottomLeftCorner = DOWN_CHAIN;
                m_topLeftCorner = LINK;
                m_topRightCorner = RIGHT_BORDER;
            } else {
                m_bottomLeftCorner = DOWN_CHAIN;
                m_bottomRightCorner = DOWN_CHAIN;
                m_topLeftCorner = LINK;
                m_topRightCorner = LINK;
            }
            break;
        case ROWS - 1:
            if (m_coords.column == 0) {
                m_topLeftCorner = TOP_LEFT;
                m_topRightCorner = UP_CHAIN;
                m_bottomRightCorner = LINK;
                m_bottomLeftCorner = LEFT_BORDER;
            } else if (m_coords.column == COLUMNS - 1) {
                m_topRightCorner = TOP_RIGHT;
                m_topLeftCorner = UP_CHAIN;
                m_bottomRightCorner = RIGHT_BORDER;
                m_bottomLeftCorner = LINK;
            } else {
                m_topRightCorner = UP_CHAIN;
                m_topLeftCorner = UP_CHAIN;
                m_bottomRightCorner = LINK;
                m_bottomLeftCorner = LINK;
            }
            break;
        default:
            if (m_coords.column == 0) {
                m_topLeftCorner = LEFT_BORDER;
                m_topRightCorner = LINK;
                m_bottomLeftCorner = LEFT_BORDER;
                m_bottomRightCorner = LINK;
                break;
            } else if (m_coords.column == COLUMNS - 1) {
                m_topLeftCorner = LINK;
                m_topRightCorner = RIGHT_BORDER;
                m_bottomRightCorner = RIGHT_BORDER;
                m_bottomLeftCorner =  LINK;
                break;
            }
            m_topLeftCorner = LINK;
            m_topRightCorner = LINK;
            m_bottomRightCorner = LINK;
            m_bottomLeftCorner = LINK;
            break;
    }
}

RenderSquare &RenderSquare::operator=(const RenderSquare &rhs) {
    m_coords = rhs.m_coords;
    m_color = rhs.m_color;
    // Wasn't like this before
    m_topLeftCorner = rhs.m_topLeftCorner;
    m_topRightCorner = rhs.m_topRightCorner;
    m_bottomLeftCorner = rhs.m_bottomLeftCorner;
    m_bottomRightCorner = rhs.m_bottomRightCorner;
    return *this;
}