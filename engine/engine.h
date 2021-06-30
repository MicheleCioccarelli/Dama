#pragma once

#include "../board/board.h"
#include "../player/player.h"
#include "../enums/ErrorType.h"
#include "../enums/GameStyle.h"

class GameEngine {
public:    Board board;

    Player whitePlayer;
    Player blackPlayer;

    ErrorType validate_move(Move move);

    ErrorType check_eat(Move move);

    void dispatch_move(Move move);

    int count_pieces(PlayerColor pColor);

    bool game_over();

    GameEngine(GameStyle gameStyle);
};
