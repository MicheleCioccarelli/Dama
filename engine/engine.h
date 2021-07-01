#pragma once

#include "../board/board.h"
#include "../player/player.h"
#include "../enums/ErrorType.h"
#include "../enums/GameStyle.h"

class GameEngine {
public:    Board board;

    Player whitePlayer;
    Player blackPlayer;

    // Returns the color of the player who performed a move
    PlayerColor determine_player (Move move);

    ErrorType validate_move(Move move);

    ErrorType check_eat(Move move);

    // The move provided is what your opponent should have done for you not to bloe his dama
    ErrorType check_blow(Move move);

    void dispatch_move(Move move);

    int count_pieces(PlayerColor pColor);

    bool game_over();

    GameEngine(GameStyle gameStyle);

};
