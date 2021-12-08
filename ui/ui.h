#pragma once

#include "../enums/MoveReturn.h"
#include "../engine/engine.h"
#include "../constants/colors_codes.h"
#include <algorithm>
#include <sstream>

#define MAX_COMMANDS 2

class UI {
public:
    // Initialize engine's names
    static void init(GameEngine& engine);

    static MoveReturn get_move(Move& move, GameEngine& engine, const PlayerColor& currentPlayer);
    // Check input lenght and see wether it is a command
    static MoveReturn check_input(const std::string& input);

    static void input_to_move(const std::string& input, Move& move);

    // Interpret the coords contained in toConvert and returns them
    static Coords convert_coords(const std::string& toConvert);

    // CHecks if a coordinate is legitimate
    static MoveReturn validate_input (const Coords& coords);

    //Given a certain player color check if the move is correct (White player can't move black pieces, ...)
    static MoveReturn check_color(Move& move, PlayerColor currentPlayer, GameEngine& engine);

    // Explains the error to the user
    static void log_error(MoveReturn error);

    // Returns true if the input was actually a command and assigns the right MoveReturn to move
    static bool validate_command(std::string& command, PlayerColor currentPlayer, Move& move);

    // Puts the contents of 2 commands in a move, uninitialized commands are ignored
    //static void command_to_move(const std::vector<Command>& commands, Move& move);

    ~UI() = default;
};