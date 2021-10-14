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
