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
    // The player has requested a summary command
    SUMMARY,
    // Someone has offered a draw, used in UI::get_move()
    DRAW_OFFER,
    // White has offered a draw
    W_DRAW_OFFER,
    // Black offered a draw
    B_DRAW_OFFER,
};