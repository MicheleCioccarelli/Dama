#pragma once

#include "../move/move.h"
#include "../enums/PlayerColor.h"
#include "../enums/MoveReturn.h"

#include <vector>
#include <iostream>

class Player {
public:
    PlayerColor color;
    std::string name;

    std::vector<Move> moves;

    void add_move(Move move);

    Player& operator=(const Player &rhs);

    // If the player is BIANCO returns NERO and vice versa
    PlayerColor other_color();

    Player(): name("nigga"), color(TRASPARENTE) {}

    Player(std::string &_name, PlayerColor _color): name(_name), color(_color) {}
};