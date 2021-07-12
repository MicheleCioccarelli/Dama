#pragma once

#include "../board/board.h"
#include "../player/player.h"
#include "../enums/GameStyle.h"
#include "../render/render.h"

class GameEngine {
public:
    Player whitePlayer;
    Player blackPlayer;
    Board board;
public:
    StdRender render;

    MoveReturn submit(const Move& move);

    static PlayerColor deduce_color(const Move &move);

    // This tests if a move between the first and last element of coords can be performed,
    // the move's type needs to be accurate
    MoveReturn validate_move(Move& move);

    MoveReturn check_eat(Move& move);

    // You have to pass in the input flagged as blowable by the user, this function tells
    // you wheter it can be blown, if yes you should call Board::blow_up();
    MoveReturn check_blow(Coords _startingCoords, Coords _endingCoords);

    void dispatch_move(const Move& move, bool isBlown);

    int count_pieces(PlayerColor pColor);

    // Convert coords from human notation no matrix notation
    static Coords convert_coords(Coords coords);

    void promote();

    bool game_over();

    GameEngine(GameStyle gameStyle, BoardTokens _tokens, SetPieces _pieces, BoardCoords _coords);
};
