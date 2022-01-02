#include "colorMatrix.h"
ColorMatrix::ColorMatrix() {
    matrix.resize(ROWS);

    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLUMNS; col++) {
            matrix[row].emplace_back(RenderSquare(Coords((ColumnNotation)col, row)));
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
            matrix[row][col].metaColor = BOARD_COLOR;
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
    matrix[coords.row][coords.column].color.northColor = color;
    matrix[coords.row][coords.column].color.eastColor = color;
    matrix[coords.row][coords.column].metaColor = color;
}


void ColorMatrix::flip_board() {
    // This is where the changes are stored
    ColorMatrix tempMatrix;

    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLUMNS; col++) {
            // Only proceed if the square is colored, and therefore needs to be moved
            if (matrix[row][col].metaColor != BOARD_COLOR) {
                std::string color = matrix[row][col].metaColor;

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
                tempMatrix.matrix[newRow][newCol].metaColor = matrix[row][col].metaColor;

                if (newRow < ROWS - 1) {
                    tempMatrix.matrix[newRow + 1][newCol].color.southColor = color;
                } else {
                    tempMatrix.matrix[newRow][newCol].color.northColor = color;
                }
                if (newCol < COLUMNS - 1) {
                    tempMatrix.matrix[newRow][newCol + 1].color.westColor = color;
                } else {
                    tempMatrix.matrix[newRow][newCol].color.eastColor = color;
                }
                tempMatrix.matrix[newRow][newCol].color.westColor = color;
                tempMatrix.matrix[newRow][newCol].color.southColor = color;
            }
        }
    }
    matrix = tempMatrix.matrix;
}

void ColorMatrix::color_board(Move &move) {
    Coords currentCoords = move.startingCoord.convert_coords();

    // Painting the first square of the move as a moving square
    paint_square(currentCoords, MOVE_COLOR);

    currentCoords = move.endingCoord.convert_coords();
    paint_square(currentCoords, MOVE_COLOR);
    Coords forwardCoords;
    if (move.type == EAT) {
        Coords previousCoords;
        int horizontalDistance = 0;
        int verticalDistance = 0;
        // If there are multiple eatings
        for (int i = 0; i < move.eatenCoords.size(); i++) {
            currentCoords = move.eatenCoords[i].convert_coords();
            if (i >= 1) {
                previousCoords = move.eatenCoords[i - 1].convert_coords();
            } else {
                previousCoords = move.startingCoord.convert_coords();
            }

            // Used to calculate where the damina ends up (also used in check_eat)
            if (i == 0) {
                verticalDistance = (currentCoords.row - previousCoords.row);
                horizontalDistance = (currentCoords.column - previousCoords.column);
            } else {
                verticalDistance = (currentCoords.row - forwardCoords.row);
                horizontalDistance = (currentCoords.column - forwardCoords.column);
            }

            forwardCoords = Coords((ColumnNotation)(currentCoords.column + horizontalDistance),
                                          currentCoords.row + verticalDistance);
            paint_square(forwardCoords, MOVE_COLOR);
            paint_square(currentCoords, EAT_COLOR);
        }
    }
    if (!move.blownCoord.is_uninitialized()) {
        currentCoords = move.blownCoord.convert_coords();
        paint_square(currentCoords, BLOW_COLOR);
    }
}
