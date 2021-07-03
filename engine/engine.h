#pragma once

#include "../board/board.h"
#include "../player/player.h"
#include "../enums/MoveReturn.h"
#include "../enums/GameStyle.h"
#include "../render/render.h"

class GameEngine {
public:
    Player whitePlayer;
    Player blackPlayer;

    StdRender render;
    Board board;

    MoveReturn submit(const Move& move);

    // This tests if a move between the first and last element of coords can be performed,
    // the move's type needs to be accurate
    MoveReturn validate_move(Move& move);

    MoveReturn check_eat(Move& move);

    // The move provided is what your opponent should have done for you not to blow his dama
    MoveReturn check_blow(Move move);

    void dispatch_move(const Move& move);

    int count_pieces(PlayerColor pColor);

    void promote();

    GameEngine(GameStyle gameStyle, BoardTokens _tokens, SetPieces _pieces, BoardCoords _coords);
};
