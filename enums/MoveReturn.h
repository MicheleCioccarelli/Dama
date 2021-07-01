#pragma once

// Used by GameEngine::validate_move()
enum MoveReturn {
    VALID,

    WHITE_SQUARE,

    TOO_FAR,

    BEHIND,

    POPULATED,

    EMPTY_START,

    EMPTY_TARGET,

    CANNIBALISM,

    TOO_BIG,

    FRIENDLY_FIRE,

    INVALID
};