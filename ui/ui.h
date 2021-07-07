#pragma once

#include "../engine/engine.h"
#include <sstream>

class UI {
public:
    static void get_move();
    static Coords convert_coords(char _column, char _row);

    ~UI() = default;
};