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

void StdRender::first_line(int columns) {
    std::cout << "   " << boardTokens.topLeft;
    for (int i = 1; i < columns; i++) {
        border(boardTokens.upChain);
    }
    border(boardTokens.topRight);
    std::cout << std::endl;
}

void StdRender::bottom_line(PlayerColor color, int columns) {
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

void StdRender::line(int columns) {
    std::cout << "   " << boardTokens.leftBorder;
    for (int i = 1; i < columns; i++) {
        border(boardTokens.link);
    }
    border(boardTokens.rightBorder);
    std::cout << std::endl;
}

void StdRender::middle(Board &b, PlayerColor color, int rows, int columns) {
    switch (color) {
        case BIANCO:
            for (int _row = rows - 1; _row >= 0; _row--) {
                std::cout << _row + 1 << "  ";
                for (int _col = 0; _col < columns; _col++) {
                    std::cout << boardTokens.verticalLine << "  " <<
                              square_resolve(Coords((ColumnNotation) _col, _row), b) << "  ";
                }
                std::cout << boardTokens.verticalLine << std::endl;
                if (_row != 0) {
                    line(columns);
                }
            }
            bottom_line(color, columns);
            break;
        case NERO:
            for (int _row = 0; _row < rows; _row++) {
                std::cout << _row + 1 << "  ";
                for (int _col = columns - 1; _col >= 0; _col--) {
                    std::cout << boardTokens.verticalLine << "  " <<
                              square_resolve(Coords((ColumnNotation) _col, _row), b) << "  ";
                }
                std::cout << boardTokens.verticalLine << std::endl;
                if (_row != columns - 1) {
                    line(columns);
                }
            }
            bottom_line(color, columns);
            break;
    }
}

void StdRender::render_board(PlayerColor color, Board &b, int rows, int columns) {
    std::cout << std::endl << std::endl;
    first_line(columns);
    middle(b, color, rows, columns);
    std::cout << std::endl << std::endl;
}

void StdRender::end_screen_table(int whitePieces, int blackPieces) {

}
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
void StdRender::end_screen() {
    std::cout << "          Gioco finito" << std::endl;
    first_line(2);
}
