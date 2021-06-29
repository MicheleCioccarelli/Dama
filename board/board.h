#pragma once

#include "../square/square.h"
#include "../enums/PlayerColor.h"
#include "../move/move.h"

#include <vector>

class Board {
public:
    std::vector<std::vector<Square>> board;

    // Put white peaces in rows 1 ~ 3 and black in the upper three
    void standard_game_initialization();

    // Color the black squares
    void colored_game_initialization();

    // The square at the starting coords' piece is copied at the eninding coords
    // and the starting square is set to VUOTA
    void execute_move(Move move);

    Board();
};
