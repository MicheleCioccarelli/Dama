#pragma once

#include "../enums/MoveData.h"
#include "../engine/engine.h"
#include "../constants/colors_codes.h"
#include <algorithm>
#include <sstream>

class UI {
public:
    // Initialize engine's names
    static void init(GameEngine& engine);

    static void handle_issue(MoveIssue issue);

    /**
     * Returns invalid if there was a problem with the move
     * */
    static MoveData get_move(Move& move, GameEngine& engine, const PlayerColor& currentPlayer);
    // Check input lenght and see wether it is a command
    static MoveData check_input(const std::string& input);

    static MoveIssue input_to_move(const std::string& input, Move& move, GameEngine& engine);

    // Interpret the coords contained in toConvert and returns them
    static Coords convert_coords(const std::string& toConvert);

    // Explains the error to the user
    static void log_error(MoveIssue error);

    // Returns true if the input was actually a command and assigns the right MoveReturn to move
    static MoveData validate_command(std::string& command, PlayerColor currentPlayer, Move& move);

    ~UI() = default;
};