#pragma once

#include <vector>
#include <iostream>

#include "../engine/engine.h"
#include "../coords/coords.h"
#include "../movecase/movecase.h"

class Command {
public:
    Coords startingCoords;
    Coords endingCoords;
    std::vector<Coords> eatenCoords;
    MoveCase type;

    // Interpret the coords contained in to Convert and put them in converted
    static void convert_coords(const std::string& toConvert, Coords& converted);

    Command(std::string& input, GameEngine& engine);
};
