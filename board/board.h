#pragma once
#include "../helpers/helpers.h"

#include <vector>

#define columns 8
#define rows 8

class Board {
public:
    std::vector<std::vector<Square>> board;

    Board();
};

class GameEngine  {
    Board board;
public:
    bool validate_move(Move move);
    void dispatch_move(Move move);
    int count_pieces(PlayerColor pColor);
    bool game_over();

    GameEngine(Board &_board)
        : board(_board) {}
};