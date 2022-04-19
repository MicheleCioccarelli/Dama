#include "color.h"

Color::Color() {
    m_northColor = BOARD_COLOR;
    m_southColor = BOARD_COLOR;
    m_eastColor = BOARD_COLOR;
    m_westColor = BOARD_COLOR;
}

bool Color::operator==(const std::string &rhs) {
    if (m_northColor == rhs && m_southColor == rhs && m_eastColor == rhs && m_westColor == rhs) {
        return true;
    }
    return false;
}

Color &Color::operator=(Color &rhs) {
    m_northColor = rhs.m_northColor;
    m_southColor = rhs.m_southColor;
    m_eastColor =  rhs.m_eastColor;
    m_westColor =  rhs.m_westColor;

    return *this;
}

Color &Color::operator=(const Color &rhs) {
    m_northColor = rhs.m_northColor;
    m_southColor = rhs.m_southColor;
    m_eastColor =  rhs.m_eastColor;
    m_westColor =  rhs.m_westColor;

    return *this;
}

void Color::operator=(std::string &rhs) {
    m_northColor = rhs;
    m_southColor = rhs;
    m_eastColor =  rhs;
    m_westColor =  rhs;
}