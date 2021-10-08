#pragma once

#include "../../move/move.h"

// This kind of move is used by Apium, it can assign every move a score and compare them
class ScoredMove : Move {
public:
    int score;

    bool operator<(const ScoredMove& rhs) const;

    bool operator>(const ScoredMove &rhs) const;

    ScoredMove(Coords _startingCoords, Coords _endingCoords, PlayerColor _color = TRASPARENTE, MoveType _type = EAT, int _score = 0)
        : Move(_startingCoords, _endingCoords, _color, _type), score(_score) {}
    ScoredMove(PlayerColor _color, MoveType _moveType = UNINITIALIZED, int _score = 0)
        : Move(_color, _moveType), score(_score) {}
};

