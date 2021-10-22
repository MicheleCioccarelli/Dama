#include "colorMatrix.h"


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
            int direction = STILL;
            int horizontalDistance = 0;
            int verticalDistance = 0;
            // If there are multiple eatings
            for (int i = 1; i < move.coords.size(); i++) {
                // See wiki for a detailed explanation
                currentCoords = move.coords[i].convert_coords();
                Coords previousCoords = move.coords[i - 1].convert_coords();

                // Used to calculate where the damina ends up (also used in check_eat)
                if (i == 1) {
                    verticalDistance = (currentCoords.row - previousCoords.row);
                    horizontalDistance = (currentCoords.column - previousCoords.column);
                } else {
                    verticalDistance = (currentCoords.row - previousCoords.row) / 2;
                    horizontalDistance = (currentCoords.column - previousCoords.column) / 2;
                }
                if (horizontalDistance == 1 && verticalDistance == 1) {
                    direction = UP_RIGHT;
                } else if (horizontalDistance == 1 && verticalDistance == -1) {
                    direction = DOWN_RIGHT;
                } else if (horizontalDistance == -1 && verticalDistance == 1) {
                    direction = UP_LEFT;
                } else if (horizontalDistance == -1 && verticalDistance == -1) {
                    direction = DOWN_LEFT;
                }

                Coords forwardCoords;

                switch (direction) {
                    case UP_RIGHT:
                        forwardCoords = Coords(static_cast<ColumnNotation>(currentCoords.column + horizontalDistance),
                                                      currentCoords.row + verticalDistance);
                        break;
                    case DOWN_RIGHT:
                        forwardCoords = Coords(static_cast<ColumnNotation>(currentCoords.column + horizontalDistance),
                                                      currentCoords.row + verticalDistance);
                        break;
                    case UP_LEFT:
                        forwardCoords = Coords(static_cast<ColumnNotation>(currentCoords.column + horizontalDistance),
                                               currentCoords.row + verticalDistance);
                        break;
                    case DOWN_LEFT:
                        forwardCoords = Coords(static_cast<ColumnNotation>(currentCoords.column + horizontalDistance),
                                               currentCoords.row + verticalDistance);
                    break;
                    default:
                        forwardCoords = Coords(static_cast<ColumnNotation>(currentCoords.column),
                                               currentCoords.row);
                }

                paint_square(forwardCoords, MOVE_COLOR);
                paint_square(currentCoords, EAT_COLOR);
            }
            break;
    }
    if (move.blownCoord != Coords(Z, 9)) {
        currentCoords = move.blownCoord.convert_coords();
        paint_square(currentCoords, BLOW_COLOR);
    }
}
