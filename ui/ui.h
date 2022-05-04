#pragma once

#include "../enums/MoveData.h"
#include "../engine/engine.h"
#include "../constants/colors_codes.h"
#include <algorithm>
#include <sstream>

namespace UI {
    // Initialize engine's names
    void init(GameEngine& engine);

    void handle_issue(MoveIssue issue);

    /**
     * Returns invalid if there was a problem with the move
     * */
    MoveData get_move(Move& move, GameEngine& engine, const PlayerColor& currentPlayer);
    // Check input lenght and see wether it is a command
    MoveData check_input(const std::string& input);

    MoveIssue input_to_move(const std::string& input, Move& move, GameEngine& engine);

    // Interpret the coords contained in toConvert and returns them RETURNS HUMAN NOTATION
    Coords convert_coords(const std::string& toConvert);

    // Explains the error to the user
    void log_error(MoveIssue error);

    // Returns true if the input was actually a command and assigns the right MoveReturn to move
    MoveData validate_command(std::string& input, PlayerColor currentPlayer);
};