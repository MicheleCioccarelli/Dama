#include "color.h"

Color::Color() {
    northColor = BOARD_COLOR;
    southColor = BOARD_COLOR;
    eastColor = BOARD_COLOR;
    westColor = BOARD_COLOR;
}

Color &Color::operator=(Color &rhs) {
    northColor = rhs.northColor;
    southColor = rhs.southColor;
    eastColor =  rhs.eastColor;
    westColor =  rhs.westColor;

    return *this;
}

Color &Color::operator=(const Color &rhs) {
    northColor = rhs.northColor;
    southColor = rhs.southColor;
    eastColor =  rhs.eastColor;
    westColor =  rhs.westColor;

    return *this;
}

void Color::operator=(std::string &rhs) {
    northColor = rhs;
    southColor = rhs;
    eastColor =  rhs;
    westColor =  rhs;
}