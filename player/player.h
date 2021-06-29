#pragma once

#include "../move/move.h"
#include "../enums/PlayerColor.h"

#include <vector>
#include <iostream>

class Player {
    std::vector<Move> moves;
public:
    PlayerColor color;
    std::string name;
    void add_move(Move move);

    Player()
            : name("Default name"), color(TRASPARENTE) {}
};
