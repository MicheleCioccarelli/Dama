#include "color.h"

Color::Color() {
    moveRows.push_back(99);
    moveColumns.push_back(99);
    eatRows.push_back(99);
    eatColumns.push_back(99);
    blowRows.push_back(99);
    blowColumns.push_back(99);
    moveType = MOVE;
}

void Color::paint_coords(std::vector<int> &rowCoords, std::vector<int>& columnCoords, Coords& coords) {
    // -1 converts to matrix-notation
    if (rowCoords[0] == 99) {
        rowCoords.clear();
    }
    if (columnCoords[0] == 99) {
        columnCoords.clear();
    }
    rowCoords.push_back(coords.row);
    columnCoords.push_back(coords.column);
}

void Color::paint_move(Move& move) {
    // The first element of a move is always a piece moving, therefore colored as moving
    paint_coords(moveRows, moveColumns, move.coords[0]);

    this->moveType = move.type.moveType;

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
