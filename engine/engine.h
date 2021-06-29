#pragma once

#include "../board/board.h"
#include "../player/player.h"
#include "../move/move.h"

class GameEngine {
    Board board;
public:
    Player whitePlayer;
    Player blackPlayer;

    bool validate_move(Move move);

    void dispatch_move(Move move);

    int count_pieces(PlayerColor pColor);

    bool game_over();

    GameEngine();
};
