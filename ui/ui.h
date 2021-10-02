#pragma once

#include "../command/command.h"
#include "../constants/colors_codes.h"
#include <algorithm>
#include <sstream>

#define MAX_COMMANDS 2

class UI {
public:
    static void init(GameEngine& engine);

    static MoveReturn get_move(Move& move, GameEngine& engine, PlayerColor currentPlayer);

    static MoveReturn check_input(const std::string& input);

    // Checks every parameter of each command and makes sure it is correct
    static MoveReturn validate_command(const std::vector<Command>& commands);
    // CHecks if a coordinate is legitimate
    static MoveReturn validate_input (const Coords& coords);

    //Given a certain player color check if the move is correct (White player can't move black pieces, ...)
    static MoveReturn check_color(Move& move, PlayerColor currentPlayer, GameEngine& engine);

    // Explains the error to the user
    static void log_error(MoveReturn error);

    // Returns true if the input was actually a command and assigns the right MoveReturn to it
    static bool dispatch_command(GameEngine& engine, std::string& command, PlayerColor currentPlayer, Move& move);

    // Puts the contents of 2 commands in a move, uninitialized commands are ignored
    static void command_to_move(const std::vector<Command>& commands, Move& move);

    ~UI() = default;
};