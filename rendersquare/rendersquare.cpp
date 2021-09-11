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

void RenderSquare::paint(const std::string &color) {
    northColor = color;
    eastColor =  color;
    westColor =  color;
    southColor = color;
}

RenderSquare &RenderSquare::operator=(const RenderSquare &rhs) {
    coords = rhs.coords;
    northColor = rhs.northColor;
    southColor = rhs.southColor;
    eastColor = rhs.eastColor;
    westColor = rhs.westColor;
    topLeftCorner = rhs.topLeftCorner;
    topRightCorner = rhs.topRightCorner;
    bottomLeftCorner = rhs.bottomLeftCorner;
    bottomRightCorner = rhs.bottomRightCorner;
    return *this;
}

ColorMatrix::ColorMatrix(const BoardTokens& tokens) {

    matrix.resize(ROWS);

    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLUMNS; col++) {
            matrix[row].emplace_back(RenderSquare(tokens, Coords((ColumnNotation)col, row)));
        }
    }
}

void ColorMatrix::clear() {
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLUMNS; col++) {
            matrix[row][col].eastColor = BOARD_COLOR;
            matrix[row][col].northColor = BOARD_COLOR;
            matrix[row][col].westColor = BOARD_COLOR;
            matrix[row][col].southColor = BOARD_COLOR;
        }
    }
}

void ColorMatrix::paint_square(Coords coords, std::string color) {
    // If the square isn't at the top of the board
    if (coords.row < ROWS - 1) {
        // Since the board is drawn from top to bottom the square above the one being colored has its southern side painted
        matrix[coords.row + 1][coords.column].southColor = color;
    }
    if (coords.column < COLUMNS - 1) {
        // Same as before because the board is drawn kinda from right to left
        matrix[coords.row][coords.column + 1].westColor = color;
    }

    matrix[coords.row][coords.column].paint(color);
}

void ColorMatrix::color_board(Move &move) {
    Coords actualCoords = Coords(move.coords)
    Coords currentCoords = move.coords[0];

    // Painting the first square of the move as a moving square
    paint_square(currentCoords, MOVE_COLOR);

    currentCoords = move.coords[1];
    switch (move.type.moveType) {
        case MOVE:
            paint_square(currentCoords, MOVE_COLOR);
            break;
        case EAT:
            paint_square(currentCoords, EAT_COLOR);
            // If there are multiple eatings
            for (int i = 2; i < move.coords.size(); i++) {
                currentCoords = move.coords[i];

                if (i % 2 == 0) {
                    paint_square(currentCoords, MOVE_COLOR);
                } else {
                    paint_square(currentCoords, EAT_COLOR);
                }
            }
            break;
    }
    if (move.blownCoord != Coords(Z, 9)) {
        currentCoords = move.blownCoord;
        paint_square(currentCoords, (std::string &) BLOW_COLOR);
    }
}

// TODO
// a function that puts the right corner pieces
// When building the top square takes priority
