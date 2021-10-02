#pragma once

// Used by GameEngine::validate_move()
enum MoveReturn {
    VALID,
    // These are used by checking funcions in GameEngine
    // You are trying to move into a white square
    WHITE_SQUARE,
    // You are trying to move by more than one square
    TOO_FAR,
    // A damina is trying to go backwards
    BEHIND,
    // You are trying to move in a square which already has a piece in it
    POPULATED,
    // You are trying to move an empty square (VUOTA)
    EMPTY_START,
    // You are trying to eat an empty square (VUOTA)
    EMPTY_TARGET,
    // A piece is trying to eat a piece of the same type (DAMA_B eats DAMA_B)
    CANNIBALISM,
    // A damina is trying to eat a damone
    TOO_BIG,
    // A piece is trying to eat a piece of the same color
    FRIENDLY_FIRE,
    // The move of the wrong type is being checked (e.g. check_eat checking a BLOW move)
    INVALID,
    // The square that needs to be reached is outside the board (row -1, column P, ...)
    OUT_OF_BOUNDS,
    // The square indicated can be blown
    BLOWABLE,
    // The square indicated cannot be blown
    ROCK_SOLID,
    // The move being checked isn't valid, but the move checker doesn't know why
    UNDEFINED,

    // These are returned by UI::check_input()

    // White resigned
    WHITE_RESIGN,
    // Black resigned
    BLACK_RESIGN,
    // You tyoed nothing
    EMPTY_MOVE,
    // A player resigned
    RESIGNED,
    // The move is too short
    TOO_SHORT,
    // You used a wrong operator (not - * x)
    WRONG_OPERATOR,
    // You are trying to move a piece of the wrong color (white moves black pieces)
    WRONG_COLOR,
    // The input received is invalid (e.g. J8-LK)
    MISINPUT,
    // The user used a help page
    HELP_PAGE,
    // The player has requested a summary command
    SUMMARY
};