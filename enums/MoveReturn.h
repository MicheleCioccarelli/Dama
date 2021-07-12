#pragma once

// Used by GameEngine::validate_move()
enum MoveReturn {
    VALID,
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

    TOO_SHORT,

    WRONG_OPERATOR
    // You are trying to move a piece of the wrong color (white moves black pieces)
    WRONG_COLOR
};