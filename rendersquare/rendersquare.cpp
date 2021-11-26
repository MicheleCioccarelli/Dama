#include "rendersquare.h"

RenderSquare::RenderSquare(Coords _coords) {
    coords = _coords;
    // Format special squares depending on their position in the board
    switch (coords.row) {
        case 0:
            if (coords.column == 0) {
                bottomLeftCorner = BOTTOM_LEFT;
                bottomRightCorner = DOWN_CHAIN;
                topLeftCorner = LEFT_BORDER;
                topRightCorner = LINK;
            } else if (coords.column == COLUMNS - 1) {
                bottomRightCorner = BOTTOM_RIGHT;
                bottomLeftCorner = DOWN_CHAIN;
                topLeftCorner = LINK;
                topRightCorner = RIGHT_BORDER;
            } else {
                bottomLeftCorner = DOWN_CHAIN;
                bottomRightCorner = DOWN_CHAIN;
                topLeftCorner = LINK;
                topRightCorner = LINK;
            }
            break;
        case ROWS - 1:
            if (coords.column == 0) {
                topLeftCorner = TOP_LEFT;
                topRightCorner = UP_CHAIN;
                bottomRightCorner = LINK;
                bottomLeftCorner = LEFT_BORDER;
            } else if (coords.column == COLUMNS - 1) {
                topRightCorner = TOP_RIGHT;
                topLeftCorner = UP_CHAIN;
                bottomRightCorner = RIGHT_BORDER;
                bottomLeftCorner = LINK;
            } else {
                topRightCorner = UP_CHAIN;
                topLeftCorner = UP_CHAIN;
                bottomRightCorner = LINK;
                bottomLeftCorner = LINK;
            }
            break;
        default:
            if (coords.column == 0) {
                topLeftCorner = LEFT_BORDER;
                topRightCorner = LINK;
                bottomLeftCorner = LEFT_BORDER;
                bottomRightCorner = LINK;
                break;
            } else if (coords.column == COLUMNS - 1) {
                topLeftCorner = LINK;
                topRightCorner = RIGHT_BORDER;
                bottomRightCorner = RIGHT_BORDER;
                bottomLeftCorner =  LINK;
                break;
            }
            topLeftCorner = LINK;
            topRightCorner = LINK;
            bottomRightCorner = LINK;
            bottomLeftCorner = LINK;
            break;
    }
}

RenderSquare &RenderSquare::operator=(const RenderSquare &rhs) {
    coords = rhs.coords;
    color = rhs.color;
    // Wasn't like this before
    topLeftCorner = rhs.topLeftCorner;
    topRightCorner = rhs.topRightCorner;
    bottomLeftCorner = rhs.bottomLeftCorner;
    bottomRightCorner = rhs.bottomRightCorner;
    return *this;
}