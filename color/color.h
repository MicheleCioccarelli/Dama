#pragma once

#include <iostream>
#include "../constants/colors_codes.h"

class Color {
public:
    std::string m_northColor = BOARD_COLOR;
    std::string m_southColor = BOARD_COLOR;
    std::string m_eastColor = BOARD_COLOR;
    std::string m_westColor = BOARD_COLOR;

    void operator=(std::string& rhs);
    Color& operator=(const Color &rhs);

    bool operator==(const std::string& rhs);

    Color& operator=(Color& rhs);

    Color();
};