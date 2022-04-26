#pragma once
#include "../enums/MoveData.h"
#include "../enums/MoveIssue.h"
#include "../fileHandler/fileHandler.h"

class CommandHandler {
public:
    /**
    * Handles command execution (help page, resigning, summary, saving).
    * Returns true if the game should be ended, gameHandler will take care of the rest
    * */
    bool static execute_command(GameEngine& gameEngine, MoveData command);
};
