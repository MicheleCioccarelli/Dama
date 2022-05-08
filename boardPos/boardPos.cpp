#include "boardPos.h"

bool BoardPos::notation_to_board(std::string &notation, Board &board) {
    // Look for an "-", the three charachters before that are information about the piece and its location.
    // If you don't find an "_" you should process the next 3 charachters if they are present and then stop

    // Delete all the pieces from the board
    board.clear();

    if (notation.size() > 4) {
        std::transform(notation.begin(), notation.end(), notation.begin(), ::toupper);
        // If notation is not empty/invalid
        std::string beingExamined;
        Coords coords;
        // Counter
        int i;

        if (notation[notation.size() - 1] != '_') {
            // Made to make things smoother
            notation.append("_");
        }

        // "_" are found every 3 character, by starting at -1 you can alays use i += 4
        for (i = 3; i < notation.size(); i += 4) {
            // Making sure the syntax is correct
            if (notation.at(i) == '_') {
                beingExamined.clear();
                // If notation is "...wB3_..." beingExamined should be "wB3"
                beingExamined.append(notation, i - 3, 3);

                // Convert the string to human-notation coords
                coords = UI::convert_coords(beingExamined.substr(1));
                // Convert coords to matrix-notation
                coords = coords.convert_coords();

                // Add the piece
                board.matrix[coords.row][coords.column].m_piece = char_to_piece(beingExamined.at(0));
            } else {
                return false;
            }
        }
        // Delete the inserted "_"
        notation.pop_back();
        return true;
    }
}

Piece BoardPos::char_to_piece(char c) {
    switch (c) {
        case 'w':
            return {BIANCO, DAMA};
        case 'W':
            return {BIANCO, DAMONE};
        case 'b':
            return {NERO, DAMA};
        case 'B':
            return {NERO, DAMONE};
        default:
            return {TRASPARENTE, VUOTA};
    }
}

std::string BoardPos::board_to_noation(const Board &board) {
    std::string output {};

    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLUMNS; col++) {
            switch (board.matrix[row][col].m_piece.m_color) {
                case TRASPARENTE:
                    break;
                case BIANCO:
                    board.matrix[row][col].m_piece.m_type == DAMA ? output += "w" : output += "W";
                    // ADD COLUMN, similar expression to the one used in RenderV2::render_columns()

                    output += (char)('A' + col);
                    // "+1" is used to convert to human notation
                    output += std::to_string(row + 1);
                    output += '_';
                    break;

                case NERO:
                    board.matrix[row][col].m_piece.m_type == DAMA ? output += "b" : output += "B";

                    // Same as above
                    output += (char)('A' + col);
                    output += std::to_string(row + 1);
                    output += '_';
                    break;
            }
        }
    }
    return output;
}