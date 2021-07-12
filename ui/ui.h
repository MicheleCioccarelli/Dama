#pragma once

#include "../command/command.h"
#include <sstream>

#define MAX_COMMANDS 2

class UI {
public:
    static void get_move(Move& move, GameEngine& engine, PlayerColor currentPlayer);

    static void log_error(MoveReturn error);

    // Puts the contents of 2 commands in a move, uninitialized commands are ignored
    static void command_to_move(const std::vector<Command>& commands, Move& move);

    ~UI() = default;
};