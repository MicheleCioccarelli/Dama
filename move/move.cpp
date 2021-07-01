#include "move.h"

Move::Move(Coords _startingCoord, Coords _endingCoord, PlayerColor _color, MoveType _moveType) {
    startingCoord = Coords(_startingCoord.column, _startingCoord.row - 1);
    endingCoord = Coords(_endingCoord.column, _endingCoord.row - 1);
    color = _color;
    type = _moveType;
}

void Move::fill_eat(const std::vector<Coords>& coords) {
    for (int i = 0; i < coords.size(); i++) {
        piecesEaten.emplace_back(coords.at(i).column, coords.at(i).row - 1);
    }
}