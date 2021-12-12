#pragma once

// Issues with a move or commands
enum MoveData {
    VALID,
    // The move was too short
    TOO_SHORT,
    // You tried to move or eat twice in one move
    DOUBLE_EVENT,
    // White resigned
    WHITE_RESIGN,
    // Black resigned
    BLACK_RESIGN,
    // You tyoed nothing
    EMPTY_MOVE,
    // A player resigned
    RESIGNED,
    // You used a wrong operator (not - * x)
    WRONG_OPERATOR,
    // You are trying to move a piece of the wrong color (white moves black pieces)
    WRONG_COLOR,
    // The input received is invalid (e.g. J8-LK)
    MISINPUT,
    // The user used a help page
    HELP_PAGE,
    // The player has requested a summary command
    SUMMARY,
    // Someone has offered a draw
    DRAW_OFFER,
    // White has offered a draw
    W_DRAW_OFFER,
    // Black offered a draw
    B_DRAW_OFFER,
    // The user wrote a move with the wrong lenght
    LENGHT_ERROR
};