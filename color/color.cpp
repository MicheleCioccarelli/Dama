#include "color.h"

Color &Color::operator=(Color &rhs) {
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