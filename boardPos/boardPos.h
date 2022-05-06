#pragma once
#include "../engine/engine.h"
#include "../ui/ui.h"


namespace BoardPos {
    // Overwrites board with the informarion contained in notation
    bool notation_to_board(std::string& notation, Board& board);

    std::string board_to_noation(const Board& board);

    // Returns the piece associated with a letter (w, W, b, B) see wiki for more details
    Piece char_to_piece(char c);
}