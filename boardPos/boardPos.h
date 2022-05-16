#pragma once
#include "../ui/ui.h"

#include "../engine/engine.h"

namespace BoardPos {
    // Overwrites board with the informarion contained in notation
    bool notation_to_board(std::string& notation, Board& board);

    /* Generates a string which describes the current board,
     * ignoring all white squares for efficency (see wiki for more info on the notation)
     * */
    std::string board_to_noation_ignoring_white_squares(const Board& board);

    // Same as the other board_to_notation function but the output includes white squares
    std::string board_to_noation_with_white_squares(const Board& board);

    // Returns the piece associated with a letter (w, W, b, B) see wiki for more details
    Piece char_to_piece(char c);

    // Returns a vector of all the squares described in the notation, used in Apium
    std::vector<Square> notation_to_squares(std::string& notation);
}