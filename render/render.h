#pragma once

#include "../board/board.h"
#include "../enums/BoardStyles.h"
#include "../enums/GameState.h"
#include "../player/player.h"
#include "../constants/colors_codes.h"
#include "../color/color.h"

struct BoardTokens {
public:
    std::string horizontalLine;
    std::string verticalLine;
    std::string topLeft;
    std::string topRight;
    std::string bottomLeft;
    std::string bottomRight;
    std::string upChain;
    std::string downChain;
    std::string rightBorder;
    std::string link;
    std::string leftBorder;
    std::string filling;

    explicit BoardTokens(BoardStyles style) {
        switch (style) {
            case NORMAL:
                horizontalLine = "═";
                verticalLine = "‖";
                topLeft = "╔";
                topRight = "╗";
                bottomLeft = "╚";
                bottomRight = "╝";
                upChain = "╦";
                downChain = "╩";
                rightBorder = "╣";
                link = "╬";
                leftBorder = "╠";
                filling = " ";
                break;
        }
    }
};

struct SetPieces {
    std::string damaB;
    std::string damaN;
    std::string damoneB;
    std::string damoneN;
    std::string vuota;
    std::string colorata;

    SetPieces(BoardStyles style) {
        switch (style) {
            case NORMAL:
                damaB = "●";
                damaN = "○";
                damoneB = "▲";
                damoneN = "Δ";
                vuota = " ";
                colorata = "█";
                break;
        }
    }
};

struct BoardCoords {
    char a;
    char b;
    char c;
    char d;
    char e;
    char f;
    char g;
    char h;

    BoardCoords(BoardStyles style) {
        switch (style) {
            case NORMAL:
                a = 'a';
                b = 'b';
                c = 'c';
                d = 'd';
                e = 'e';
                f = 'f';
                g = 'g';
                h = 'h';
                break;
        }
    }
};

class StdRender {
    BoardTokens boardTokens;
    SetPieces setPieces;
    BoardCoords boardCoords;
public:
    void border(std::string &lastChar);
    void first_line(int columns);
    void bottom_line(PlayerColor color, int columns);
    void line(int columns);
    void middle(Board &board, PlayerColor color, int row, int column);
    std::string square_resolve(Coords coords, Board &board);

    void render_board(Player &currentPlayer, Board &b, int rows, int columns);

    // Draws name on the screen center in a space as big as longerName
    void center_name(std::string& name, int longerName);
    static void padding(int numberOfSpaces);
    // Result is how the game ended/who is the winner
    void end_screen(int whitePieces, int blackPieces, Player& whitePlayer,
                    Player& blackPlayer, GameState result);

    static void help_page();

    StdRender(BoardTokens &_boardTokens, SetPieces &_setPieces, BoardCoords &_boardcoords)
        : boardTokens(_boardTokens), setPieces(_setPieces), boardCoords(_boardcoords) {}
};
