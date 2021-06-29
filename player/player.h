#pragma once

#include "../move/move.h"

class Player {
    std::vector<Move> moves;
public:
    PlayerColor color;
    std::string name;
    void add_move(Move move);

    Player()
            : name("Default name"), color(UNINITIALIZED) {}
};