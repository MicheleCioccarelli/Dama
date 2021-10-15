#pragma once

enum PieceType {
    DAMA, DAMONE,
    // Colorata is ignored by GameEngine::check_move() Currently it is used for highlightin certain squares
    COLORATA,
    VUOTA
};
