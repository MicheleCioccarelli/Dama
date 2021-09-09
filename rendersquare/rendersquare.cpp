#include "rendersquare.h"

RenderSquare::RenderSquare(const BoardTokens& tokens, Coords _coords) {
    _coords = coords;
    // Format special squares depending on their position in the board
    switch (coords.row) {
        case 0:
            if (coords.column == 0) {
                bottomLeftCorner = tokens.bottomLeft;
                bottomRightCorner = tokens.downChain;
            } else if (coords.column == COLUMNS - 1) {
                bottomRightCorner = tokens.bottomRight;
                bottomLeftCorner = tokens.downChain;
            } else {
                bottomLeftCorner = tokens.downChain;
            }
            break;
        case ROWS - 1:
            if (coords.column == 0) {
                topLeftCorner = tokens.topLeft;
                topRightCorner = tokens.upChain;
                bottomRightCorner = tokens.leftBorder;
            } else if (coords.column == COLUMNS - 1) {
                topRightCorner = tokens.topRight;
                topLeftCorner = tokens.upChain;
                bottomRightCorner = tokens.rightBorder;
            } else {
                topRightCorner = tokens.upChain;
            }
            break;
        default:
            if (coords.column == 0) {
                bottomLeftCorner = tokens.leftBorder;
            } else if (coords.column == COLUMNS - 1) {
                bottomRightCorner = tokens.rightBorder;
                break;
            }
            bottomLeftCorner = tokens.link;
    }
}

void RenderSquare::paint(const std::string &color) {
    north_side = color;
    east_side =  color;
    west_side =  color;
    south_side = color;
}

ColorMatrix::ColorMatrix(const BoardTokens& tokens, const Coords coords) {
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLUMNS; col++) {
            matrix[row][col] = RenderSquare(tokens, Coords((ColumnNotation)col, row));
        }
    }
}

void ColorMatrix::clear() {
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {
            matrix[row][col].east_side = BOARD_COLOR;
            matrix[row][col].north_side = BOARD_COLOR;
            matrix[row][col].west_side = BOARD_COLOR;
            matrix[row][col].south_side = BOARD_COLOR;
        }
    }
}

void ColorMatrix::paint_square(Coords coords, std::string& color) {
    // If the square isn't at the top of the board
    if (coords.column < columns - 1) {
        // Since the board is drawn from top to bottom the square above the one being colored has its southern side painted
        matrix[coords.row][coords.column + 1].south_side = color;
    }
    matrix[coords.row][coords.column].paint(color);
}

void ColorMatrix::color_board(Move &move) {
    Coords currentCoords = move.coords[0];

    // Painting the first square of the move as a moving square
    paint_square(currentCoords, (std::string &) MOVE_COLOR);

    currentCoords = move.coords[1];
    switch (move.type.moveType) {
        case MOVE:
            paint_square(currentCoords, (std::string &) MOVE_COLOR);
            break;
        case EAT:
            paint_square(currentCoords, (std::string &) EAT_COLOR);
            // If there are multiple eatings
            for (int i = 2; i < move.coords.size(); i++) {
                currentCoords = move.coords[i];

                if (i % 2 == 0) {
                    paint_square(currentCoords, (std::string &) MOVE_COLOR);
                } else {
                    paint_square(currentCoords, (std::string &) EAT_COLOR);
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
