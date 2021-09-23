#pragma once

#include "../board/board.h"
#include "../player/player.h"
#include "../enums/GameStyle.h"
#include "../enums/GameState.h"
#include "../render/render.h"

class GameEngine {
public:
    Player whitePlayer;
    Player blackPlayer;
    Board board;
public:
    RenderV2 render;

    MoveReturn submit(const Move& move);

    PlayerColor deduce_color(Move &move);

    // This tests if a move between the first and last element of coords can be performed,
    // the move's type needs to be accurate
    MoveReturn check_move(Move& move);

    MoveReturn check_eat(Move& move);

    // You have to pass in the input flagged as blowable by the user, this function tells
    // you wheter it can be blown, if yes you should call Board::blow_up();
    MoveReturn check_blow(Coords _startingCoords, Coords _endingCoords);

    void dispatch_move(const Move& move, bool isBlown);

    int count_pieces(PlayerColor pColor);

    void promote();

    // These funcion are used in game_over()
    // and check every possible move, if at least one is found they return true
    bool simulate_damina(Piece piece, Coords coords);
    bool simulate_damona(Piece piece, Coords coords);

    GameState game_over();

    GameEngine(GameStyle gameStyle, BoardStyles tokenStyle, BoardStyles pieceStyle, BoardStyles coordStyle);
};
