#pragma once

#include <iostream>
#include <vector>

#include "../move/move.h"
#include "../coords/coords.h"
#include "../color/color.h"
#include "../constants/board_assets.h"
#include "../constants/number_constants.h"

class RenderSquare {
public:
    Coords coords;
    /*
    // The color of each side
    std::string northColor = BOARD_COLOR;
    std::string southColor = BOARD_COLOR;
    std::string eastColor = BOARD_COLOR;
    std::string westColor = BOARD_COLOR;
    */
    // Stores information about what the sides of the square should look like
    Color color;

    std::string topLeftCorner;
    std::string topRightCorner;
    std::string bottomLeftCorner;
    std::string bottomRightCorner;

    // Colors all of the square's sides in the color
    void paint(std::string& _color);

    RenderSquare& operator=(const RenderSquare& rhs);

    RenderSquare(const BoardTokens& tokens, Coords _coords);
};

class ColorMatrix {
public:
    std::vector<std::vector<RenderSquare>> matrix;

    void paint_square(Coords coords, std::string color);

    // Flips tha matrix color upside down, used when rendering black's POV
    void flip_board();

    // Color the right squares according to the given move, color refers to which side of the board you are rendering (BIANCO or NERO)
    void color_board(Move& move);

    // Set every square's color to white
    void clear();

    ColorMatrix() = default;

    ColorMatrix(const BoardTokens& tokens);
};