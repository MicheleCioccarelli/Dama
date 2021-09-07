#include "render.h"
// ======= RENDER =======

void StdRender::calculate_offsets(Color &color, int rows, int columns) {
    for (int i = 0; i < color.moveRows.size(); i++) {
        if (color.moveRows[i] == 99) {
            break;
        }
        // This is the distance from the top of the board to the square which needs to be colored
        color.moveRows[i] = rows - color.moveRows[i];
    }
    for (int i = 0; i < color.moveColumns.size(); i++) {
        if (color.moveColumns[i] == 99) {
            break;
        }
        color.moveColumns[i] = color.moveColumns[i];
    }
    for (int i = 0; i < color.eatRows.size(); i++) {
        if (color.eatRows[i] == 99) {
            break;
        }
        color.eatRows[i] = rows - color.eatRows[i];
    }
    for (int i = 0; i < color.eatColumns.size(); i++) {
        if (color.eatColumns[i] == 99) {
            break;
        }
        color.eatColumns[i] = color.eatColumns[i] + 1;
    }
    for (int i = 0; i < color.blowRows.size(); i++) {
        if (color.blowRows[i] == 99) {
            break;
        }
        color.blowRows[i] = rows - color.blowRows[i];
    }
    for (int i = 0; i < color.blowColumns.size(); i++) {
        if (color.blowColumns[i] == 99) {
            break;
        }
        color.blowColumns[i] = color.blowColumns[i] + 1;
    }
}

std::string StdRender::square_resolve(Coords coords, Board &board) {
    switch (board.matrix[coords.row][coords.column].piece.color) {
        case BIANCO:
            switch (board.matrix[coords.row][coords.column].piece.type) {
                case DAMA:
                    return setPieces.damaB;
                case DAMONE:
                    return setPieces.damoneB;
                case VUOTA:
                    return setPieces.vuota;
                case COLORATA:
                    return setPieces.colorata;
            }
            case NERO:
                switch (board.matrix[coords.row][coords.column].piece.type) {
                    case DAMA:
                        return setPieces.damaN;
                    case DAMONE:
                        return setPieces.damoneN;
                    case VUOTA:
                        return setPieces.vuota;
                    case COLORATA:
                        return setPieces.colorata;
                }
        default:
            return " ";
    }
}

void StdRender::border(std::string &lastChar, std::string lineColor) {
    std::cout << lineColor << boardTokens.horizontalLine << boardTokens.horizontalLine << boardTokens.horizontalLine
              << boardTokens.horizontalLine << boardTokens.horizontalLine << RESET << lastChar;
}

void StdRender::first_line(int columns) {
    std::cout << "   " << boardTokens.topLeft;
    for (int i = 1; i < columns; i++) {
        border(boardTokens.upChain);
    }
    border(boardTokens.topRight);
    std::cout << std::endl;
}

void StdRender::bottom_line(PlayerColor playerColor, int columns, int colorOffset, std::string lineColor) {
    std::cout << "   " << boardTokens.bottomLeft;
    for (int i = 1; i < columns; i++) {
        if (i != colorOffset) {
            border(boardTokens.downChain);
        } else {
            border(boardTokens.downChain, lineColor);
        }
    }
    border(boardTokens.bottomRight);
    std::cout << std::endl;

    if (playerColor == BIANCO) {
        std::cout << "      " << boardCoords.a << "  " << "   " << boardCoords.b << "     "
                  << boardCoords.c << "     " << boardCoords.d << "     " << boardCoords.e
                  << "     " << boardCoords.f << "     " << boardCoords.g << "     " << boardCoords.h << std::endl;
    } else {
        std::cout << "      " << boardCoords.h << "  " << "   " << boardCoords.g << "     "
                  << boardCoords.f << "     " << boardCoords.e << "     " << boardCoords.d
                  << "     " << boardCoords.c << "     " << boardCoords.b << "     " << boardCoords.a << std::endl;
    }

}

void StdRender::line(int columns, int colorOffset, const std::string& lineColor) {
    std::cout << "   " << boardTokens.leftBorder;
    for (int i = 1; i < columns; i++) {
        if (i != colorOffset) {
            border(boardTokens.link);
        } else {
            border(boardTokens.link, lineColor);
        }
    }
    border(boardTokens.rightBorder);
    std::cout << std::endl;
}

void StdRender::middle(Board &b, PlayerColor playerColor, int rows, int columns, Color colorOffsets) {
    std::string CURRENT_COLOR = BOARD_COLOR;
    // This is true when a square is being colored
    bool wasColored = false;
    bool shouldColor = false;
    // This is for coloring the top and bottom of a square
    bool lineColor = false;
    int lineIndex = 0;
    // This is the place in the coloring vector of a row that should be colored, so it can be
    // paired with the corresponding column (if you found moveRow[1] you have to color moveColumn[1])
    int vectorPosition = 0;

    switch (playerColor) {
        case BIANCO:
            switch (colorOffsets.moveType) {
                // Only check the move vectors in colorOffsets and do normal rendering
                case MOVE:
                    for (int _row = rows - 1; _row >= 0; _row--) {
                        for (int i = 0; i < colorOffsets.moveColumns.size(); i++) {
                            if (colorOffsets.moveRows[i] <= 0) {
                                // This tells the columns that this is the right row
                                vectorPosition = i;
                                shouldColor = true;
                                // Exclude this index from being colored in the future
                                colorOffsets.moveRows[i] = 9;
                            } else {
                                // This is the line above the one which should be printed, now we color the top
                                if (colorOffsets.moveRows[i] == 1) {
                                    lineColor = true;
                                    lineIndex = i;
                                }
                                // moveRows says how many rows you have to count before coloring one
                                // when you get to 0 you are on the row you should color
                                colorOffsets.moveRows[i]--;

                            }
                        }
                        // Print row number
                        std::cout << _row + 1 << "  ";

                        for (int _col = 0; _col < columns; _col++) {
                            if (shouldColor == true) {
                                if (colorOffsets.moveColumns[vectorPosition] > 0) {
                                    colorOffsets.moveColumns[vectorPosition]--;
                                } else {
                                    CURRENT_COLOR = MOVE_COLOR;
                                    wasColored = true;
                                    shouldColor = false;
                                }
                            }
                            std::cout << CURRENT_COLOR << boardTokens.verticalLine << RESET;
                            if (wasColored == false) {
                                CURRENT_COLOR = BOARD_COLOR;
                            }
                            std::cout << "  " <<
                                      square_resolve(Coords((ColumnNotation) _col, _row), b) << "  ";
                            wasColored = false;
                        }
                        std::cout << CURRENT_COLOR << boardTokens.verticalLine << RESET << std::endl;
                        CURRENT_COLOR = BOARD_COLOR;

                        if (_row != 0) {
                            if (lineColor) {
                                line(columns, colorOffsets.moveColumns[lineIndex] + 1, MOVE_COLOR);
                                lineColor = false;
                            } else {
                                line(columns);
                            }
                            shouldColor = false;
                            wasColored = false;
                        }
                    }
                    bottom_line(BIANCO, columns);
                    break;
            }
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
            bottom_line(NERO, columns);
            break;
    }
}

/*  If currentPlayer's move vector is empty, this is the first turn, and you should display white,
 *  all the other times thw last layer is being passed in, so render can look at the last move made
 *  and color the board accordingly. The board will be displayes on the opposite side because it
 *  is now the opponent's turn
*/
void StdRender::render_board(Player &currentPlayer, Board &b, int rows, int columns) {
    Color colorOffsets = Color();

    // When there isn't a move there is nothing to offset
    if (!currentPlayer.moves.empty()) {
        for (int i = 0; i < currentPlayer.moves.size(); i++) {
            colorOffsets.paint_move(currentPlayer.moves[i]);
            calculate_offsets(colorOffsets, rows, columns);
        }
    }
    std::cout << std::endl << std::endl;
    first_line(columns);
    middle(b, currentPlayer.color, rows, columns, colorOffsets);
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
        case DRAW:
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
    for (int i = 1; i < 22; i++) {
        std::cout << "════";
    }
    std::cout << std::endl << HBBLU <<"Pagina di aiuto" << RESET << std::endl << std::endl;
    std::cout << "uso: [colonna][riga][operatore][colonna][riga]" << std::endl << std::endl;
    std::cout << "Colonne: A, B, C, D, E, F, G, H" << std::endl;
    std::cout << "Righe:   1, 2, 3, 4, 5, 6, 7, 8" << std::endl << std::endl;

    std::cout << "Operatori:" << std::endl;
    padding(3);
    std::cout << BCYN << "*" << RESET;
    padding(3);
    std::cout << "Soffio, deve essere il primo comando se è usato";
    padding(2);
    std::cout << "|  Esempio: A3*B4 C3-D4" << std::endl;

    padding(3);
    std::cout << BCYN << "-" << RESET;
    padding(3);
    std::cout << "Movimento, cambia casella di 1 alla volta";
    padding(8);
    std::cout << "|  Esempio: A3-B4" << std::endl;

    padding(3);
    std::cout << BCYN << "x" << RESET;
    padding(3);
    std::cout << "Mangiare";
    padding(41);
    std::cout << "|  Esempio: A3xB4" << std::endl;
    for (int i = 1; i < 22; i++) {
        std::cout << "════";
    } std::cout << std::endl;
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
