#pragma once

#include "../coords/coords.h"
#include "../enums/MoveType.h"

class Command {
    Coords startingCoords;
    Coords endingCoords;
public:
    MoveType type;
    Command();
};
