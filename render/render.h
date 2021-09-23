#pragma once

#include "../board/board.h"
#include "../enums/GameState.h"
#include "../player/player.h"
#include "../color/color.h"

#include "../rendersquare/rendersquare.h"

class StdRender {
    BoardTokens boardTokens;
    SetPieces setPieces;
    BoardCoords boardCoords;
public:
    // lineColor is the color in which the text is displayed, white by default, if the square being
    // drawn was involved in movement it will be of color MOVE_COLOR, and so on
    void border(std::string &lastChar, std::string lineColor = WHT);

    // This function is only given an int colorOffset because it only needs to draw
    void first_line(int columns);

    void bottom_line(PlayerColor playerColor, int columns, int colorOffset = 0, std::string lineColor = WHT);
    void line(int columns, int colorOffset = 0, const std::string& lineColor = WHT);
    void middle(Board &board, PlayerColor playerColor, int row, int column, Color colorOffsets);
    std::string square_resolve(Coords coords, Board &board);

    /* This funciton analyzes color's vectors and replaces their coordinates
     * with offsets which can be used when drawing
     */
    void calculate_offsets(Color& color, int rows, int columns);

    void render_board(Player &currentPlayer, Board &b, int rows, int columns);

    // Draws name on the screen center in a space as big as longerName
    static void center_name(std::string& name, int longerName);
    static void padding(int numberOfSpaces);
    // Result is how the game ended/who is the winner
    void end_screen(int whitePieces, int blackPieces, Player& whitePlayer,
                    Player& blackPlayer, GameState result) const;

    static void end_screen();

    StdRender(BoardTokens &_boardTokens, SetPieces &_setPieces, BoardCoords &_boardcoords)
        : boardTokens(_boardTokens), setPieces(_setPieces), boardCoords(_boardcoords) {}
};

class RenderV2 {
    BoardTokens boardTokens;
    SetPieces setPieces;
    BoardCoords boardCoords;

    ColorMatrix colorMatrix;
public:
    RenderV2(BoardStyles tokenStyle, BoardStyles pieceStyle, BoardStyles coordStyle);

    // Returns a symbol corresponding to the piece held at a square from board in coords
    std::string square_resolve(Coords coords, Board &board) const;
    // Draw the top part of the squares in row, according to colorMatrix
    void render_top(int row, PlayerColor color);
    // Renders the contents of the middle of the squares of board in row
    void render_middle(int row, Board& board, PlayerColor color);
    // Renders the bottom corners along with horizontal lines of square from colorMatrix at a certain row
    void render_bottom(int row);
    // Renders a board on the right side, if no move is provided the result is all BOARD_COLOR (see constants/color_codes.h)
    void render_board(Board& board, PlayerColor color, Move move = Move());
    // Renders the right row number based on color
    static void render_rows(PlayerColor color, int row);
    // Draws A ~ H on the bottom of the board
    void render_columns(PlayerColor color) const;

    void padding(int numberOfSpaces);

    void help_page();
};
