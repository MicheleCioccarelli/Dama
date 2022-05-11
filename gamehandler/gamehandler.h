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
    void two_player_game(GameEngine& engine);
    void cli_error(cliCase error);
};
