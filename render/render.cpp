#include "render.h"

/*  PIECES LABLE
 * horizontalLine = "═"
 * topLeft = "╔"
 * topRight = "╗"
 * bottomLeft = "╚"
 * bottomRight = "╝"
 * upChain = "╦"
 * downChain = "╩"
 * link = "╬"
 * filling = " "
 *leftBorder = "╠"
 * rightBorder "╣"
 *
 * tokens.[0] = horizontalLine
 * tokens.[1](verticalLine);
 * tokens.[2](topLeft);
 * tokens.[3](topRight);
 * tokens.[4](bottomLeft);
 * tokens.[5](bottomRight);
 * tokens.[6](upChain);
 * tokens.[7](downChain);
 * tokens.[8](rightBorder);
 * tokens.[9](link);
 * tokens.[10](leftBorder);
 * tokens.[11](filling);
*/
// ======= RENDER =======
std::string StdRender::square_resolve(Coords coords, Board &board) {
    switch (board.matrix[coords.row][coords.column].piece) {

        case DAMA_B:
            return setPieces.damaB;
        case DAMA_N:
            return setPieces.damaN;
        case DAMONE_B:
            return setPieces.damoneB;
        case DAMONE_N:
            return setPieces.damoneN;
        case VUOTA:
            return setPieces.vuota;
        case COLORATA:
            return setPieces.colorata;
        default:
            return "sup motherfucker";
    }
}

void StdRender::border(std::string &lastChar) {
    std::cout << boardTokens.horizontalLine << boardTokens.horizontalLine << boardTokens.horizontalLine
              << boardTokens.horizontalLine << boardTokens.horizontalLine << lastChar;
}

void StdRender::first_line() {
    std::cout << "   " << boardTokens.topLeft;
    for (int i = 1; i < columns; i++) {
        border(boardTokens.upChain);
    }
    border(boardTokens.topRight);
    std::cout << std::endl;
}

void StdRender::bottom_line(PlayerColor color) {
    std::cout << "   " << boardTokens.bottomLeft;
    for (int i = 1; i < columns; i++) {
        border(boardTokens.downChain);
    }
    border(boardTokens.bottomRight);
    std::cout << std::endl;

    if (color == BIANCO) {
        std::cout << "      " << boardCoords.a << "  " << "   " << boardCoords.b << "     "
                  << boardCoords.c << "     " << boardCoords.d << "     " << boardCoords.e
                  << "     " << boardCoords.f << "     " << boardCoords.g << "     " << boardCoords.h << std::endl;
    } else {
        std::cout << "      " << boardCoords.h << "  " << "   " << boardCoords.g << "     "
                  << boardCoords.f << "     " << boardCoords.e << "     " << boardCoords.d
                  << "     " << boardCoords.c << "     " << boardCoords.b << "     " << boardCoords.a << std::endl;
    }

}

void StdRender::line() {
    std::cout << "   " << boardTokens.leftBorder;
    for (int i = 1; i < columns; i++) {
        border(boardTokens.link);
    }
    border(boardTokens.rightBorder);
    std::cout << std::endl;
}

void StdRender::middle(Board &b, PlayerColor color) {
    switch (color) {
        case BIANCO:
            for (int row = rows - 1; row >= 0; row--) {
                std::cout << row + 1 << "  ";
                for (int col = 0; col < columns; col++) {
                    std::cout << boardTokens.verticalLine << "  " <<
                              square_resolve(Coords((ColumnNotation) col, row), b) << "  ";
                }
                std::cout << boardTokens.verticalLine << std::endl;
                if (row != 0) {
                    line();
                }
            }
            bottom_line(color);
            break;
        case NERO:
            for (int row = 0; row < rows; row++) {
                std::cout << row + 1 << "  ";
                for (int col = columns - 1; col >= 0; col--) {
                    std::cout << boardTokens.verticalLine << "  " <<
                              square_resolve(Coords((ColumnNotation) col, row), b) << "  ";
                }
                std::cout << boardTokens.verticalLine << std::endl;
                if (row != columns - 1) {
                    line();
                }
            }
            bottom_line(color);
            break;
    }
}

void StdRender::render_board(PlayerColor color, Board &b) {
    std::cout << std::endl << std::endl;
    first_line();
    middle(b, color);
    std::cout << std::endl << std::endl;
}
