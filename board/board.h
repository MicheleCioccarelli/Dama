#pragma once

#include "../square/square.h"
#include "../enums/PlayerColor.h"
#include "../move/move.h"

#include <vector>

class Board {
public:
    int rows;
    int columns;

    std::vector<std::vector<Square>> matrix;

    // Put white peaces in rows 1 ~ 3 and black in the upper three
    void standard_game_initialization();

    // Color the black squares
    void colored_game_initialization();

    // Every piece is a damone
    void damone_game_initialization();

    // Every square is empty
    void empty_game_initialization();

    void execute_move(Move& move);

    // This function puts blownCoords in the move's blownCoords and deletes the blown coords from the board
    void blow_up(Move& move);

    void edit(Coords coords, Piece _piece);

    Board(int _rows, int _columns);
};
