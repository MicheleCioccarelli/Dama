#pragma once

#include "../move/move.h"
#include "../enums/PlayerColor.h"

#include <vector>
#include <iostream>

class Player {
public:
    std::vector<Move> moves;

    PlayerColor color;
    std::string name;
    void add_move(Move move);

    Player()
            : name("Default name"), color(TRASPARENTE) {}
};
