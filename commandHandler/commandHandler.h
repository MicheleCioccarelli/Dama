#pragma once
#include "../enums/MoveData.h"
#include "../enums/MoveIssue.h"
#include "../fileHandler/fileHandler.h"
#include "../helpPages/helpPages.h"
#include "../boardPos/boardPos.h"

namespace CommandHandler {
    /**
    * Handles command execution (help page, resigning, summary, saving).
    * Returns true if the game should be ended, gameHandler will take care of the rest
    * */
    bool execute_game_command(GameEngine& gameEngine, MoveData command);
};
