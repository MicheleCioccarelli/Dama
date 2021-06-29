#pragma once

// Used by GameEngine::validate_move()
enum ErrorType {
    VALID, WHITE_SQUARE, TOO_FAR, BEHIND, POPULATED, EMPTY_START
};
