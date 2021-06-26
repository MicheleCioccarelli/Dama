#pragma once

#include "../board/board.h"

class Move {
public:
    Coords startingCoord;
    Coords endingCoord;
    Piece piece;

    // 1 is automatically removed from the row number, so you can write it using normal notation
    Move(Coords _startingCoord, Coords _endingCoord, Piece _piece);
};

class Player {
    std::vector<Move> moves;
public:
    PlayerColor color;
    std::string name;
    void add_move(Move move);

    Player()
        : name("Default name"), color(UNINITIALIZED) {}
};

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
