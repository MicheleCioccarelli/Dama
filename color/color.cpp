#include "color.h"

void Color::paint_coords(std::vector<int> &rowCoords, std::vector<int>& columnCoords, Coords& coords) {
    // -1 converts to matrix-notation
    rowCoords.push_back(coords.row);
    columnCoords.push_back(coords.column);
}

void Color::paint_move(Move& move) {
    // The first element of a move is always a piece moving, therefore colored as moving
    paint_coords(moveRows, moveColumns, move.coords[0]);

    if (move.type == MOVE) {
        paint_coords(moveRows, moveColumns, move.coords[1]);
    } else if (move.type == EAT) {
        // If the move is eat, you fill the eating coordinates vectors
        paint_coords(eatRows, eatColumns, move.coords[1]);
    }
    if (move.blownCoord != Coords(Z, 9)) {
        paint_coords(blowRows, blowColumns, move.blownCoord);
    }
}
