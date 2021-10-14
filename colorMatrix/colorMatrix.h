#pragma once

#include "../rendersquare/rendersquare.h"

class ColorMatrix {
public:
    std::vector<std::vector<RenderSquare>> matrix;

    void paint_square(Coords coords, std::string color);

    // Flips the board colors from white POV to black POV
    void flip_board();

    // Color the right squares according to the given move, color refers to which side of the board you are rendering (BIANCO or NERO)
    void color_board(Move& move);

    // Set every square's color to white
    void clear();

    ColorMatrix() = default;

    ColorMatrix(const BoardTokens& tokens);
};