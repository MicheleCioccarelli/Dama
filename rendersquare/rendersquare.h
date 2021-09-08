#pragma once

#include <iostream>
#include <vector>

#include "../move/move.h"
#include "../coords/coords.h"
#include "../constants/colors_codes.h"
#include "../constants/board_assets.h"

class RenderSquare {
public:
    Coords coords;
    // The color of each side
    std::string north_side = BOARD_COLOR;
    std::string south_side = BOARD_COLOR;
    std::string east_side = BOARD_COLOR;
    std::string west_side = BOARD_COLOR;

    std::string topLeftCorner;
    std::string topRightCorner;
    std::string bottomLeftCorner;
    std::string bottomRightCorner;

    // Colors all of the square's sides in the color
    void paint(const std::string& color);

    RenderSquare(const BoardTokens& tokens, Coords _coords);
};

class RenderBoard {
    int rows;
    int columns;

    std::vector<std::vector<RenderSquare>> matrix;

    void paint_square(Coords coords, std::string& color);

    // Color the right squares according to the given move
    void color_board(Move& move);

    // Set every square's color to white
    void clear();

    RenderBoard(int _rows, int _columns, const BoardTokens& tokens, const Coords coords);
};