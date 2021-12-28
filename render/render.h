#pragma once

#include "../board/board.h"
#include "../enums/GameState.h"
#include "../player/player.h"
#include "../color/color.h"
#include "../colorMatrix/colorMatrix.h"

#include <time.h>

class RenderV2 {
    ColorMatrix colorMatrix;
public:
    RenderV2();

    // Returns a symbol corresponding to the piece held at a square from board in coords
    static std::string square_resolve(Coords coords, Board &board) ;
    // Draw the top part of the squares in row, according to colorMatrix
    void render_top(int row);
    // Renders the contents of the middle of the squares of board in row
    void render_middle(int row, Board& board, PlayerColor color);
    // Renders the bottom corners along with horizontal lines of square from colorMatrix at a certain row
    void render_bottom(int row);
    // Renders a board on the right side, if no move is provided the result is all BOARD_COLOR (see constants/color_codes.h)
    void render_board(Board& board, PlayerColor color, Move move = Move());
    // Renders the right row number based on color
    static void render_rows(PlayerColor color, int row);
    // Draws A ~ H on the bottom of the board
    static void render_columns(PlayerColor color) ;

    // Clear all the colors from the board (everything becomes BOARD_COLOR)
    void clear();

    // Render a number of spaces
    static void padding(int numberOfSpaces);

    // Used in end_screen() to put names in the middle of the table
    static void center_name(std::string& name, int longerName, const std::string& color = BOARD_COLOR);

    static void end_screen(int whitePieces, int blackPieces, Player whitePlayer, Player blackPlayer, GameState result, time_t start) ;

    // Called when the executable receives wrong command line arguments
    static void cli_help_page();

    // Called when in a game someone types "aiuto"
    static void help_page();
};
