#pragma once

#include <iostream>
#include "../constants/colors_codes.h"

class Color {
public:
    std::string northColor = BOARD_COLOR;
    std::string southColor = BOARD_COLOR;
    std::string eastColor = BOARD_COLOR;
    std::string westColor = BOARD_COLOR;

    void operator=(std::string& rhs);
    Color& operator=(const Color &rhs);

    bool operator==(const std::string& rhs);

    Color& operator=(Color& rhs);

    Color();
};