#pragma once

#include "../helpPages/helpPages.h"
#include "../ui/ui.h"
#include "../enums/cliCases.h"
#include "../commandHandler/commandHandler.h"

// Runs the game
class GameHandler {
public:
    static void debug(GameEngine& engine);
    static void two_player_game(GameEngine& engine);
    static void cli_error(cliCase error);
};
