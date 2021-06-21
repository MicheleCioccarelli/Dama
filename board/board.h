#pragma once
#include "../helpers/helpers.h"

#include <vector>

#define columns 8
#define rows 8

class Move {
    Coords startingCoord;
    Coords endingCoord;
    Piece piece;
public:
    Move(Coords _startingCoord, Coords _endingCoord, Piece _piece)
            : startingCoord(_startingCoord), endingCoord(_endingCoord), piece(_piece) {}
};


class Board {
public:
    std::vector<std::vector<Square>> board;

    void render();
    void standard_game_initialization();

    Board();
};

class GameEngine  {
    Board board;
public:
    bool validate_move(Move move);
    void dispatch_move(Move move);
    int count_pieces(PlayerColor pColor);
    bool game_over();

    GameEngine(Board &_board)
        : board(_board) {}
};

class Player {
public:
    PlayerColor pColor;
    std::string name;

    std::vector<Move> moves;

    Player(std::string &_name, PlayerColor _pColor)
            : name(_name), pColor(_pColor) {}
};
