#pragma once

// Used by move check functions to validate a certain move
enum MoveIssue {
    // There are no issues
    ALL_GOOD = 0,
    // You are trying to move into a white square
    WHITE_SQUARE = 1,
    // You are trying to move by more than one square
    TOO_FAR = 2,
    // A damina is trying to go backwards
    BEHIND = 3,
    // You are trying to move in a square which already has a piece in it
    POPULATED = 4,
    // You are trying to move an empty square (VUOTA)
    EMPTY_START = 5,
    // You are trying to eat an empty square (VUOTA)
    EMPTY_TARGET = 6,
    // A piece is trying to eat a piece of the same type (DAMA_B eats DAMA_B)
    CANNIBALISM = 7,
    // A piece is trying to eat itself
    TOO_BIG = 8,
    // A piece is trying to eat a piece of the same color
    FRIENDLY_FIRE = 9,
    // The move of the wrong type is being checked (e.g. check_eat checking a BLOW move)
    INVALID = 10,
    // The square that needs to be reached is outside the board (row -1, column P, ...)
    OUT_OF_BOUNDS = 11,
    // The square indicated can be blown
    BLOWABLE = 12,
    // The square indicated cannot be blown, the move is therefore invalid
    ROCK_SOLID = 13,
    // The move being checked isn't valid, but the move checker doesn't know why
    UNDEFINED = 14,
    // The user entered an empty move
    NO_MOVE = 15,
    // The user tried to move or eat twice in one move
    DOUBLE_EVENT = 16,
    // The input received is invalid (e.g. J8-LK)
    MISINPUT = 17,
    // The move was too short
    TOO_SHORT = 18,
    // The user typed nothing
    EMPTY_MOVE = 19,
    // You are trying to move a piece of the wrong color (white moves black pieces)
    WRONG_COLOR = 20,
    // You used a wrong operator (not - * x)
    WRONG_OPERATOR = 21,
};