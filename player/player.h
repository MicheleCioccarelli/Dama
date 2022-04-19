#pragma once

#include "../move/move.h"
#include "../enums/PlayerColor.h"
#include "../enums/MoveData.h"

#include <vector>
#include <iostream>

class Player {
public:
    PlayerColor m_color;
    std::string m_name;

    std::vector<Move> m_moves;

    void add_move(Move move);

    Player& operator=(const Player &rhs);

    // If the player is BIANCO returns NERO and vice versa
    PlayerColor other_color();

    Player(): m_name(""), m_color(TRASPARENTE) {}

    Player(std::string &name, PlayerColor color): m_name(name), m_color(color) {}
};