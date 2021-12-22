#pragma once

#include <vector>
#include "../coords/coords.h"
#include "../piece/Piece.h"
#include "../enums/MoveType.h"
#include "../enums/MoveData.h"

class Move {
public:
    std::vector<Coords> eatenCoords;
    // New pieces are added by move check functions
    std::vector<Piece> eatenPieces;

    Coords startingCoord;
    Coords endingCoord;

    Coords blownCoord;

    PlayerColor playerColor;

    MoveType type;
    // Any additional information about the move, like its type of command
    MoveData status;
    // Add a coordinate to eatenCoords
    void add_coords(const Coords & _coords);

    Move();

    Move(Coords _startingCoord, Coords _endingCoord, PlayerColor _color = TRASPARENTE, MoveType _type = EAT);

    explicit Move(PlayerColor _color, MoveType _type = UNINITIALIZED);

    Move& operator= (const Move& rhs);
};
