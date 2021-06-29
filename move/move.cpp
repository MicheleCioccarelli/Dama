#include "move.h"

Move::Move(Coords _startingCoord, Coords _endingCoord, Piece _piece) {
    startingCoord = Coords(_startingCoord.column, _startingCoord.row - 1);
    endingCoord = Coords(_endingCoord.column, _endingCoord.row - 1);
    piece = _piece;
}
