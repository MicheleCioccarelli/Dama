#include "rendersquare.h"

RenderSquare::RenderSquare(const BoardTokens& tokens, Coords _coords) {
    coords = _coords;
    // Format special squares depending on their position in the board
    switch (coords.row) {
        case 0:
            if (coords.column == 0) {
                bottomLeftCorner = tokens.bottomLeft;
                bottomRightCorner = tokens.downChain;
                topLeftCorner = tokens.leftBorder;
                topRightCorner = tokens.link;
            } else if (coords.column == COLUMNS - 1) {
                bottomRightCorner = tokens.bottomRight;
                bottomLeftCorner = tokens.downChain;
                topLeftCorner = tokens.link;
                topRightCorner = tokens.rightBorder;
            } else {
                bottomLeftCorner = tokens.downChain;
                bottomRightCorner = tokens.downChain;
                topLeftCorner = tokens.link;
                topRightCorner = tokens.link;
            }
            break;
        case ROWS - 1:
            if (coords.column == 0) {
                topLeftCorner = tokens.topLeft;
                topRightCorner = tokens.upChain;
                bottomRightCorner = tokens.link;
                bottomLeftCorner = tokens.leftBorder;
            } else if (coords.column == COLUMNS - 1) {
                topRightCorner = tokens.topRight;
                topLeftCorner = tokens.upChain;
                bottomRightCorner = tokens.rightBorder;
                bottomLeftCorner = tokens.link;
            } else {
                topRightCorner = tokens.upChain;
                topLeftCorner = tokens.upChain;
                bottomRightCorner = tokens.link;
                bottomLeftCorner = tokens.link;
            }
            break;
        default:
            if (coords.column == 0) {
                topLeftCorner = tokens.leftBorder;
                topRightCorner = tokens.link;
                bottomLeftCorner = tokens.leftBorder;
                bottomRightCorner = tokens.link;
                break;
            } else if (coords.column == COLUMNS - 1) {
                topLeftCorner = tokens.link;
                topRightCorner = tokens.rightBorder;
                bottomRightCorner = tokens.rightBorder;
                bottomLeftCorner = tokens.link;
                break;
            }
            topLeftCorner = tokens.link;
            topRightCorner = tokens.link;
            bottomRightCorner = tokens.link;
            bottomLeftCorner = tokens.link;
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