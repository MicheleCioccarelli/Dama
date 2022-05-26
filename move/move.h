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
    bool m_wasPromotion = false;
    std::vector<Coords> eatenCoords;
    // New pieces are added by move check functions
    std::vector<Piece> eatenPieces;

    Coords startingCoord;
    Coords endingCoord;

    Coords blownCoord;
    // The last coord of a blow move (in the move A3*B4_f6-e5 it would be B4)
    Coords endingBlowCoord;

    PlayerColor playerColor;

    // Writes in the parameter a string version of the move (es "A3-B4"), adds 1 to the row number
    std::string toStringHuman() const;

    // Doesn't add 1 to the row number
    std::string toStringMatrix() const;

    MoveType moveType;
    // Any additional information about the move, like its type of command
    MoveData m_status;
    // Add a coordinate to eatenCoords
    void push_coords(const Coords & _coords);

    // Delete  a coordinate from eatenCoords
    void pop_coords();

    // Delete all the elements
    void clear();

    // Calculates the ending coordinate, useful in an EAT move where you have startingCoords and eatenCoords
    // Also copied from GameEngine::calculate_forward
    void calculate_endingCoord();
    // Convert all the coordinates in the move from human-notation to matrix-notation
    void convert_all();

    bool is_empty() const;

    bool is_misinput() const;

    Move();

    Move(Coords startingCoords, PlayerColor color, MoveType type);

    Move(Coords startingCoords, MoveType type);

    Move(Coords startingCoord, Coords endingCoord, MoveType type = EAT, PlayerColor color = TRASPARENTE);

    Move(Coords startingCoord, Coords endingCoord, PlayerColor color = TRASPARENTE, MoveType type = EAT);

    explicit Move(PlayerColor color, MoveType type = UNINITIALIZED);

    Move& operator= (const Move& rhs);
};
