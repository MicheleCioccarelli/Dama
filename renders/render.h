#pragma once

#include "../board/board.h"
#include "../enums/BoardStyles.h"

class Render {
public:
    virtual void border(std::string &lastChar) = 0;

    virtual void first_line() = 0;

    virtual void bottom_line(PlayerColor color) = 0;

    virtual void line() = 0;

    virtual void middle(Board &board, PlayerColor color) = 0;

    virtual std::string square_resolve(Coords coords, Board &board) = 0;

    virtual void board(PlayerColor color, Board &b) = 0;

    virtual ~Render() {}
};

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

class StdRender : public Render {
    BoardTokens boardTokens;
    SetPieces setPieces;
    BoardCoords boardCoords;
public:
    virtual void border(std::string &lastChar) override;
    virtual void first_line() override;
    virtual void bottom_line(PlayerColor color) override;
    virtual void line() override;
    virtual void middle(Board &board, PlayerColor color) override;
    virtual std::string square_resolve(Coords coords, Board &board) override;
    virtual void board(PlayerColor color, Board &b) override;

    StdRender(BoardTokens &_boardTokens, SetPieces &_setPieces, BoardCoords &_boardcoords)
        : boardTokens(_boardTokens), setPieces(_setPieces), boardCoords(_boardcoords) {}

    virtual ~StdRender() {}
};
