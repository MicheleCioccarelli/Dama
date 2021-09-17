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

ColorMatrix::ColorMatrix(const BoardTokens& tokens) {
    matrix.resize(ROWS);

    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLUMNS; col++) {
            matrix[row].emplace_back(RenderSquare(tokens, Coords((ColumnNotation)col, row)));
        }
    }
}

void RenderSquare::paint(std::string &_color) {
    this->color = _color;
}

void ColorMatrix::clear() {
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLUMNS; col++) {
            matrix[row][col].color.northColor = BOARD_COLOR;
            matrix[row][col].color.southColor = BOARD_COLOR;
            matrix[row][col].color.eastColor = BOARD_COLOR;
            matrix[row][col].color.westColor = BOARD_COLOR;
        }
    }
}

void ColorMatrix::paint_square(Coords coords, std::string color) {
    // To better understand this look at how the board is drawn in RenderV2::render_middle()

    // If the square isn't at the top of the board
    if (coords.row < ROWS - 1) {
        // Since the board is drawn from top to bottom the square above the one being colored has its southern side painted
        matrix[coords.row + 1][coords.column].color.southColor = color;
    } else {
        matrix[coords.row][coords.column].color.northColor = color;
    }
    if (coords.column < COLUMNS - 1) {
        // Same as before because the board is drawn kinda from right to left
        matrix[coords.row][coords.column + 1].color.westColor = color;
    } else {
        matrix[coords.row][coords.column].color.eastColor = color;
    }
    matrix[coords.row][coords.column].color.westColor = color;
    matrix[coords.row][coords.column].color.southColor = color;
}

void ColorMatrix::flip_board() {
    // This is where the changes are stored
    std::vector<std::vector<RenderSquare>> tempMatrix;
    tempMatrix = matrix;

    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLUMNS; col++) {
            // Only proceed if the square is colored, and therefore needs to be moved
            if (matrix[row][col].color.southColor != BOARD_COLOR) {
                std::string color = matrix[row][col].color.southColor;
                // CLEANUP
                // Wiping the squares near the colored one: undoing some of paint_square()
                if (row < ROWS - 1) {
                    // Since the board is drawn from top to bottom the square above the one being colored has its southern side painted
                    tempMatrix[row + 1][col].color.southColor = BOARD_COLOR;
                    matrix[row + 1][col].color.southColor = BOARD_COLOR;
                } else {
                    tempMatrix[row][col].color.northColor = BOARD_COLOR;
                    matrix[row][col].color.northColor = BOARD_COLOR;
                }
                if (col < COLUMNS - 1) {
                    // Same as before because the board is drawn kinda from right to left
                    tempMatrix[row][col + 1].color.westColor = BOARD_COLOR;
                    matrix[row][col + 1].color.westColor = BOARD_COLOR;
                } else {
                    tempMatrix[row][col].color.eastColor = BOARD_COLOR;
                    matrix[row][col].color.eastColor = BOARD_COLOR;
                }
                tempMatrix[row][col].color.southColor = BOARD_COLOR;
                tempMatrix[row][col].color.westColor = BOARD_COLOR;

                // FLIPPING
                int newRow = ROWS - 1;
                int newCol = COLUMNS - 1;

                if (row != 0 && row != ROWS - 1) {
                    newRow = ROWS - 1 - row;
                } else if (row == ROWS - 1) {
                    newRow = 0;
                }
                if (col != 0 && col != COLUMNS - 1) {
                    newCol = COLUMNS - 1 - col;
                } else if (col == COLUMNS - 1) {
                    newCol = 0;
                }
                // RE COLORING
                tempMatrix[newRow][newCol].color = matrix[row][col].color;

                if (newRow < ROWS - 1) {
                    tempMatrix[newRow + 1][newCol].color.southColor = color;
                } else {
                    tempMatrix[newRow][newCol].color.northColor = color;
                }
                if (newCol < COLUMNS - 1) {
                    tempMatrix[newRow][newCol + 1].color.westColor = color;
                } else {
                    tempMatrix[newRow][newCol].color.eastColor = color;
                }
                matrix[row][col].color.southColor = BOARD_COLOR;
                matrix[row][col].color.westColor = BOARD_COLOR;
            }
        }
    }
    matrix = tempMatrix;
}

void ColorMatrix::color_board(Move &move) {
    Coords currentCoords = move.coords[0].convert_coords();

    // Painting the first square of the move as a moving square
    paint_square(currentCoords, MOVE_COLOR);

    currentCoords = move.coords[1].convert_coords();
    switch (move.type.moveType) {
        case MOVE:
            paint_square(currentCoords, MOVE_COLOR);
            break;
        case EAT:
            paint_square(currentCoords, EAT_COLOR);
            // If there are multiple eatings
            for (int i = 2; i < move.coords.size(); i++) {
                currentCoords = move.coords[i].convert_coords();

                if (i % 2 == 0) {
                    paint_square(currentCoords, MOVE_COLOR);
                } else {
                    paint_square(currentCoords, EAT_COLOR);
                }
            }
            break;
    }
    if (move.blownCoord != Coords(Z, 9)) {
        currentCoords = move.blownCoord.convert_coords();
        paint_square(currentCoords, BLOW_COLOR);
    }
}
