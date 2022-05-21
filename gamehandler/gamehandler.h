#pragma once

#include "../helpPages/helpPages.h"
#include "../ui/ui.h"
#include "../enums/cliCases.h"
#include "../commandHandler/commandHandler.h"
#include "../boardPos/boardPos.h"
#include "../apium/apium/apium.h"

// Runs the game
namespace GameHandler {
    void debug(GameEngine& engine);

    // Plays back the game saved in the file provided. Returns 0 if everything worked correctly
    int play_back(const std::string& gameLocation);
    void two_player_game(GameEngine& engine);
    void cli_error(cliCase error);
};
