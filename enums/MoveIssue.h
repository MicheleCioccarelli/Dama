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
    // The square that needs to be reached is outside the board (row -1, column P, ...)
    OUT_OF_BOUNDS = 11,
    // The square indicated can be blown
    BLOWABLE = 12,
    // You tried to blow your opponent's piece but their last move was of EAT type
    WRONG_LAST_MOVE = 13,
    // You tried to blow but your opponent could not have made the move you propose
    ROCK_SOLID = 14,
    // You tried to go back in time but not enough moves have been made yet
    NOT_ENOUGH_MOVES = 15,
    // The move being checked isn't valid, but the move checker doesn't know why
    UNDEFINED = 16,
    // The user entered an empty move
    NO_MOVE = 17,
    // The user tried to move or eat twice in one move
    DOUBLE_EVENT = 18,
    // The input received is invalid (e.g. J8-LK)
    MISINPUT = 19,
    // The move was too short
    TOO_SHORT = 20,
    // The user typed nothing
    EMPTY_MOVE = 21,
    // You are trying to move a piece of the wrong color (white moves black pieces)
    WRONG_COLOR = 22,
    // You used a wrong operator (not - * x)
    WRONG_OPERATOR = 23,
    // You tried to blow but the move you proposed wasn't of type EAT
    WRONG_TYPE,
};