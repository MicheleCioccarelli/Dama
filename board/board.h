#pragma once

#include "../helpers/helpers.h"

#include <vector>

enum PlayerColor {
    BIANCO, NERO, UNINITIALIZED
};

class Move {
public:
    Coords startingCoord;
    Coords endingCoord;
    Piece piece;

    // 1 is automatically removed from the row number, so you can write it using normal notation
    Move(Coords _startingCoord, Coords _endingCoord, Piece _piece)
    {
        startingCoord = Coords(_startingCoord.column, _startingCoord.row - 1);
        endingCoord = Coords(_endingCoord.column, _endingCoord.row - 1);
        piece = _piece;
    }
};


class Board {
public:
    std::vector<std::vector<Square>> board;

    // Put white peaces in rows 1 ~ 3 and black in the upper three
    void standard_game_initialization();

    // Color the black squares
    void colored_game_initialization();

    void execute_move(Move move);

    Board();
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

    GameEngine() {}
};
