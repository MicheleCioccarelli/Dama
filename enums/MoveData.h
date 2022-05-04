#pragma once

// Issues with a move or commands
enum MoveData {
    VALID,
    INVALID,
    // White resigned
    WHITE_RESIGN,
    // Black resigned
    BLACK_RESIGN,
    // A player resigned, used in UI::get_move()
    RESIGNED,
    // The user used a help page
    HELP_PAGE,
    // The user wants to see more info about the commands
    COMMAND_HELP_PAGE,
    // The player has requested a summary command
    SUMMARY,
    // Command to print the move order that lead up to the current position
    MOVEORDER,
    // Overwrite the current board using BoardPos notation
    BOARDPOS,
    // Command to print out the current board position using boardPos notation
    GENERATEBOARDPOS,
    // Someone has offered a draw, used in UI::get_move()
    DRAW_OFFER,
    // White has offered a draw
    W_DRAW_OFFER,
    // Black offered a draw
    B_DRAW_OFFER,
    // The current game data should be stored in a file
    SAVE,
    // You want to terminate the program
    QUIT,
};