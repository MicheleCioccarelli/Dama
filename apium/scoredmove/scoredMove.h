#pragma once

#include "../../move/move.h"

// This kind of move is used by Apium, it can assign every move a score and compare them
class ScoredMove : Move {
private:
    float m_score;
public:
    ScoredMove(Coords _startingCoords, Coords _endingCoords, PlayerColor _color = TRASPARENTE, MoveType _type = EAT, float score = 0)
            : Move(_startingCoords, _endingCoords, _color, _type), m_score(score) {}

    ScoredMove(PlayerColor _color, MoveType _moveType = UNINITIALIZED, float score = 0)
            : Move(_color, _moveType), m_score(score) {}

    float get_score() const;

    void set_score(float newScore);

    bool operator<(const ScoredMove& rhs) const;

    bool operator>(const ScoredMove &rhs) const;


};

