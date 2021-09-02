#pragma once

#include "../move/move.h"
#include <vector>

class Color {
public:
    std::vector<int> moveRows;
    std::vector<int> moveColumns;
    std::vector<int> eatRows;
    std::vector<int> eatColumns;
    std::vector<int> blowRows;
    std::vector<int> blowColumns;

    // This function converts coords from human-notation to matrix-notation and stores them in the vectors
    void paint_coords(std::vector<int>& rowCoords, std::vector<int>& columnCoords, Coords& coords);
    // This function organizes a move in all of the color class' vectors
    void paint_move(Move& move);

    Color() = default;
};