#pragma once

#include <vector>
#include "../coords/coords.h"
#include "../piece/Piece.h"
#include "../enums/MoveType.h"
#include "../enums/MoveData.h"

class Move {
private:
    // Copied from GameEngine
    static Coords calculate_forward(const Coords& startingCoords, const Coords& endingCoords);
public:
    // If this move lead to a promotion of its piece
    bool wasPromotion = false;
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
    void push_coords(const Coords & _coords);

    // Delete  a coordinate from eatenCoords
    void pop_coords();

    // Calculates the ending coordinate, useful in an EAT move where you have startingCoords and eatenCoords
    // Also copied from GameEngine::calculate_forward
    void calculate_endingCoord();
    // Convert all the coordinates in the move from human-notation to matrix-notation
    void convert_all();

    bool is_empty() const;

    bool is_misinput() const;

    Move();

    Move(Coords _startingCoords, PlayerColor _color, MoveType _type);

    Move(Coords _startingCoords, MoveType _type);

    Move(Coords _startingCoord, Coords _endingCoord, MoveType _type = EAT, PlayerColor _color = TRASPARENTE);

    Move(Coords _startingCoord, Coords _endingCoord, PlayerColor _color = TRASPARENTE, MoveType _type = EAT);

    explicit Move(PlayerColor _color, MoveType _type = UNINITIALIZED);

    Move& operator= (const Move& rhs);
};
