#pragma once

#include <iostream>
#include <vector>

#include "../move/move.h"
#include "../coords/coords.h"
#include "../constants/colors_codes.h"
#include "../constants/board_assets.h"
#include "../constants/number_constants.h"

class RenderSquare {
public:
    Coords coords;
    // The color of each side
    std::string northColor = BOARD_COLOR;
    std::string southColor = BOARD_COLOR;
    std::string eastColor = BOARD_COLOR;
    std::string westColor = BOARD_COLOR;

    std::string topLeftCorner;
    std::string topRightCorner;
    std::string bottomLeftCorner;
    std::string bottomRightCorner;

    // Colors all of the square's sides in the color
    void paint(const std::string& color);

    RenderSquare& operator=(const RenderSquare& rhs);

    RenderSquare(const BoardTokens& tokens, Coords _coords);
};

class ColorMatrix {
public:
    std::vector<std::vector<RenderSquare>> matrix;

    void paint_square(Coords coords, std::string& color);

    // Color the right squares according to the given move
    void color_board(Move& move);

    // Set every square's color to white
    void clear();

    ColorMatrix() = default;

    ColorMatrix(const BoardTokens& tokens);
};