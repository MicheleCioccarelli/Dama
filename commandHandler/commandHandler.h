#pragma once
#include "../enums/MoveData.h"
#include "../engine/engine.h"
#include "../enums/MoveIssue.h"

class CommandHandler {
public:
    /**
    * Handles command execution (help page, resigning, summary).
    * Returns true if the game should be ended and changes gameResult to tell game_over() what to do
    * */
    bool static execute_command(GameEngine& gameEngine, MoveData command);
};
