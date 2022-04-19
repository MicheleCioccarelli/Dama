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
    Coords m_coords;

    // Stores information about what the sides of the square should look like
    Color m_color;

    // The color a square is supposed to be renderd as, regardless of what the other colors say
    std::string m_metaColor;

    std::string m_topLeftCorner;
    std::string m_topRightCorner;
    std::string m_bottomLeftCorner;
    std::string m_bottomRightCorner;

    // Colors all of the square's sides in the color
    void paint(std::string& color);

    RenderSquare& operator=(const RenderSquare& rhs);

    RenderSquare(Coords coords);
};
