#pragma once

#include "../command/command.h"
#include <algorithm>
#include <sstream>

#define MAX_COMMANDS 2

class UI {
public:
    // Starts a standalone game
    static void start_game();

    static void init(GameEngine& engine);

    static MoveReturn get_move(Move& move, GameEngine& engine, PlayerColor currentPlayer);

    // Checks every parameter of each command and makes sure it is correct
    static MoveReturn validate_command(const std::vector<Command>& commands);
    // CHecks if a coordinate is legitimate
    static MoveReturn validate_input (const Coords& coords);

    //Given a certain player color check if the move is correct (White player can't move black pieces, ...)
    static MoveReturn check_color(Move& move, PlayerColor currentPlayer, GameEngine& engine);

    static void log_error(MoveReturn error);

    // Puts the contents of 2 commands in a move, uninitialized commands are ignored
    static void command_to_move(const std::vector<Command>& commands, Move& move);

    ~UI() = default;
};