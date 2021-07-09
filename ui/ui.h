#pragma once

#include "../command/command.h"
#include <sstream>

#define MAX_COMMANDS 2

class UI {
public:
    static void get_move(Move& move, GameEngine& engine);

    // Puts the contents of 2 commands in a move, uninitialized commands are ignored
    static MoveCase command_to_move(const std::vector<Command>& commands, Move& move);

    ~UI() = default;
};