#pragma once

#include "../square/square.h"
#include "../enums/PlayerColor.h"
#include "../move/move.h"

#include "../constants/number_constants.h"

#include <array>

class Board {
public:
    std::array<std::array<Square, COLUMNS>, ROWS> matrix;

    // Deletes all the pieces on the board
    void clear();

    // Put white peaces in rows 1 ~ 3 and black in the upper three
    void standard_game_initialization();

    // Color the black squares
    void colored_game_initialization();

    // Every piece is a damone
    void damone_game_initialization();

    // Every square is empty
    void empty_game_initialization();
    // After a move gets executed it's perfect, every parameter is in place.
    void execute_move(Move& move);

    // This function puts blownCoords in the move's blownCoords and deletes the blown coords from the board
    void blow_up(Move& move);

    void edit_matrix_notation(Coords coords, Piece _piece);

    void edit_human_notation(Coords coords, Piece _piece);

    Board();
};
