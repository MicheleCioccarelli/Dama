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
    std::cout << std::endl;
}

void StdRender::center_name(std::string& name, int longerName) {
    if (name.size() == longerName) {
        std::cout << name;
    } else {
        int diff = name.size() - longerName;
        if (diff < 0) {
            diff = -diff;
        }

        for (int i = 0; i < diff / 2; i++) {
            std::cout << " ";
        }

        std::cout << name;

        if (diff % 2 == 0) {
            for (int i = 0; i < diff / 2; i++) {
                std::cout << " ";
            }
        } else {
            for (int i = 0; i < (diff / 2) + (diff % 2); i++) {
                std::cout << " ";
            }
        }
    }
}

void StdRender::padding(int numberOfSpaces) {
    for (int i = 0; i < numberOfSpaces; i++) {
        std::cout << " ";
    }
}

void StdRender::end_screen(int whitePieces, int blackPieces, Player& whitePlayer,
                           Player& blackPlayer, GameState result) {
    std::vector<std::string> strings;
    strings.emplace_back("Nome");
    strings.emplace_back("Pezzi");
    strings.emplace_back("Mosse");

    switch (result) {
        case WHITE_WIN:
            strings.emplace_back("Vittoria bianca");
            break;
        case BLACK_WIN:
            strings.emplace_back("Vittoria nera");
            break;
        case STALEMATE:
            strings.emplace_back("Pareggio");
            break;
        case PATTA:
            strings.emplace_back("Patta");
            break;
        case GOOD:
            strings.emplace_back("Il gioco non è ancora finito in teoria");
            break;
    }

    // This defines the width of the table
    int longerName;
    if (whitePlayer.name.size() >= blackPlayer.name.size()) {
        longerName = whitePlayer.name.size();
    } else {
        longerName = blackPlayer.name.size();
    }
    if (longerName < 5) {
        longerName = 5;
    }

    padding(7);
    center_name(strings[3], longerName + 13);
    std::cout << std::endl << std::endl;

    padding(5);
    // Drawing the names
    std::cout << " ";
    center_name(strings[0], longerName + 2);
    center_name(strings[1], 7);
    center_name(strings[2], 8);
    std::cout << std::endl;

    padding(5);
    // Top line
    std::cout << boardTokens.topLeft;
    for (int i = 0; i < longerName + 2; i++) {
        std::cout << boardTokens.horizontalLine;
    }
    std::cout << boardTokens.upChain;
    // Pieces block
    for (int i = 0; i < 6; i ++) {
        std::cout << boardTokens.horizontalLine;
    }
    std::cout << boardTokens.upChain;
    // Number of moves
    for (int i = 0; i < 6; i++) {
        std::cout << boardTokens.horizontalLine;
    }
    std::cout << boardTokens.topRight;

    padding(5);
    // White name
    std::cout << std::endl;
    padding(5);
    std::cout << boardTokens.verticalLine << " ";
    center_name(whitePlayer.name, longerName);
    std::cout << " " << boardTokens.verticalLine;

    // White number of pieces
    if (whitePieces < 10) {
        std::cout << "   " << whitePieces << "  ";
    } else {
        std::cout << "  " << whitePieces << "  ";
    }

    // White number of moves
    if (whitePlayer.moves.size() <= 9) {
        std::cout << boardTokens.verticalLine << "   " << whitePlayer.moves.size() << "  ";
    } else {
        std::cout << boardTokens.verticalLine << "  " << whitePlayer.moves.size() << "  ";
    }
    std::cout << boardTokens.verticalLine;

    // Middle line
    std::cout << std::endl,
    padding(5);
    std::cout << boardTokens.leftBorder;
    for (int i = 0 ; i < longerName + 2; i++) {
        std::cout << boardTokens.horizontalLine;
    } std::cout << boardTokens.link;
    for (int i = 0 ; i < 6; i++) {
        std::cout << boardTokens.horizontalLine;
    } std::cout << boardTokens.link;
    for (int i = 0 ; i < 6; i++) {
        std::cout << boardTokens.horizontalLine;
    } std::cout << boardTokens.rightBorder;

    // Black information
    std::cout << std::endl;
    padding(5);
    std::cout << boardTokens.verticalLine << " ";
    center_name(blackPlayer.name, longerName);
    std::cout << " " << boardTokens.verticalLine;

    if (blackPieces < 10) {
        std::cout << "   " << blackPieces << "  ";
    } else {
        std::cout << "  " << blackPieces << "  ";
    }

    if (blackPlayer.moves.size() <= 9) {
        std::cout << boardTokens.verticalLine << "   " << blackPlayer.moves.size() << "  ";
    } else {
        std::cout << boardTokens.verticalLine << "  " << blackPlayer.moves.size() << "  ";
    }
    std::cout << boardTokens.verticalLine << std::endl;

    padding(5);
    // Closing line
    std::cout << boardTokens.bottomLeft;
    for (int i = 0; i < longerName + 2; i++) {
        std::cout << boardTokens.horizontalLine;
    }
    std::cout << boardTokens.downChain;
    // Pieces block
    for (int i = 0; i < 6; i ++) {
        std::cout << boardTokens.horizontalLine;
    }
    std::cout << boardTokens.downChain;
    // Number of moves
    for (int i = 0; i < 6; i++) {
        std::cout << boardTokens.horizontalLine;
    }
    std::cout << boardTokens.bottomRight << std::endl;
}

void StdRender::help_page() {
    std::cout << std::endl << "Pagina di aiuto" << std::endl << std::endl;
    std::cout << "uso: [colonna][riga][operatore][colonna][riga]" << std::endl << std::endl;
    std::cout << "Colonne: A, B, C, D, E, F, G, H" << std::endl;
    std::cout << "Righe:   1, 2, 3, 4, 5, 6, 7, 8" << std::endl << std::endl;

    std::cout << "Operatori:" << std::endl;
    padding(3);
    std::cout << "*";
    padding(3);
    std::cout << "Soffio, deve essere il primo comando se è usato";
    padding(2);
    std::cout << "|  Esempio: A3*B4 C3-D4" << std::endl;

    padding(3);
    std::cout << "-";
    padding(3);
    std::cout << "Movimento, cambia casella di 1 alla volta";
    padding(8);
    std::cout << "|  Esempio: A3-B4" << std::endl;

    padding(3);
    std::cout << "x";
    padding(3);
    std::cout << "Mangiare";
    padding(41);
    std::cout << "|  Esempio: A3xB4" << std::endl;
}

/*  PIECES LABEL
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
