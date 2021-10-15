#pragma once

#include "../enums/BoardStyles.h"

struct BoardTokens {
    std::string horizontalLine {"═"};
    std::string verticalLine {"‖"};
    std::string topLeft {"╔"};
    std::string topRight {"╗"};
    std::string bottomLeft {"╚"};
    std::string bottomRight {"╝"};
    std::string upChain {"╦"};
    std::string downChain {"╩"};
    std::string rightBorder {"╣"};
    std::string link {"╬"};
    std::string leftBorder {"╠"};
    std::string filling {" "};

//    explicit BoardTokens(BoardStyles style = NORMAL) {
//        switch (style) {
//            case NORMAL:
//                horizontalLine = "═";
//                verticalLine = "‖";
//                topLeft = "╔";
//                topRight = "╗";
//                bottomLeft = "╚";
//                bottomRight = "╝";
//                upChain = "╦";
//                downChain = "╩";
//                rightBorder = "╣";
//                link = "╬";
//                leftBorder = "╠";
//                filling = " ";
//                break;
//        }
//    }
};

struct SetPieces {
    std::string damaB {"●"};
    std::string damaN {"○"};
    std::string damoneB {"▲"};
    std::string damoneN {"Δ"};
    std::string vuota {' '};
    std::string colorata {"█"};

//    SetPieces(BoardStyles style) {
//        switch (style) {
//            case NORMAL:
//                damaB = "●";
//                damaN = "○";
//                damoneB = "▲";
//                damoneN = "Δ";
//                vuota = " ";
//                colorata = "█";
//                break;
//        }
//    }
};

struct BoardCoords {
    char a {'a'};
    char b {'b'};
    char c {'c'};
    char d {'d'};
    char e {'e'};
    char f {'f'};
    char g {'g'};
    char h {'h'};

//    BoardCoords(BoardStyles style) {
//        switch (style) {
//            case NORMAL:
//                a = 'a';
//                b = 'b';
//                c = 'c';
//                d = 'd';
//                e = 'e';
//                f = 'f';
//                g = 'g';
//                h = 'h';
//                break;
//        }
//    }
};

