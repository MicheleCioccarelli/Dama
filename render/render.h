#pragma once

#include "../board/board.h"
#include "../enums/BoardStyles.h"

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
    void first_line();
    void bottom_line(PlayerColor color);
    void line();
    void middle(Board &board, PlayerColor color);
    std::string square_resolve(Coords coords, Board &board);
    void render_board(PlayerColor color, Board &b);

    StdRender(BoardTokens &_boardTokens, SetPieces &_setPieces, BoardCoords &_boardcoords)
        : boardTokens(_boardTokens), setPieces(_setPieces), boardCoords(_boardcoords) {}
};
