#pragma once

#include "../render/render.h"
#include "../ui/ui.h"
#include "../enums/cliCases.h"

// Runs the game
class GameHandler {
public:
    static int two_player_game(GameEngine& engine);
    static void cli_error(cliCase error);
};
