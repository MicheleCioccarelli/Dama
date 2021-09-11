#include "render.h"

// RENDER V2

RenderV2::RenderV2(BoardTokens &_boardTokens, SetPieces &_setPieces, BoardCoords &_boardcoords)
    : boardTokens(_boardTokens), setPieces(_setPieces), boardCoords(_boardcoords) {
    colorMatrix = ColorMatrix(boardTokens);
}

void RenderV2::render_top(int row, PlayerColor color) {
    if (color == BIANCO) {
        for (int i = 0; i < COLUMNS; i++) {
            RenderSquare currentSquare = colorMatrix.matrix[row][i];

            std::cout << BOARD_COLOR << currentSquare.topLeftCorner << RESET;

            std::cout << currentSquare.northColor << boardTokens.horizontalLine << boardTokens.horizontalLine <<
                      boardTokens.horizontalLine << boardTokens.horizontalLine << boardTokens.horizontalLine << RESET;
        }
    } else {
        // When you draw black side you have to flip the contents of the squares but keep the corners untouched
        // so you draw corners like before but flip the color in the middle
        for (int i = 0, colorRow = 7 - row; i < COLUMNS; i++) {
            RenderSquare currentCorner = colorMatrix.matrix[row][i];
            RenderSquare currentColor = colorMatrix.matrix[colorRow][i];

            std::cout << BOARD_COLOR << currentCorner.topLeftCorner << RESET;

            std::cout << currentColor.northColor << boardTokens.horizontalLine << boardTokens.horizontalLine <<
                      boardTokens.horizontalLine << boardTokens.horizontalLine << boardTokens.horizontalLine << RESET;
        }
    }
    std::cout << BOARD_COLOR << colorMatrix.matrix[row][COLUMNS - 1].topRightCorner << RESET;
}

void RenderV2::render_bottom(int row) {
    for (int i = 0; i < COLUMNS; i++) {
        RenderSquare currentSquare = colorMatrix.matrix[row][i];

        std::cout << BOARD_COLOR << currentSquare.bottomLeftCorner << RESET;

        std::cout << currentSquare.northColor << boardTokens.horizontalLine << boardTokens.horizontalLine <<
                  boardTokens.horizontalLine << boardTokens.horizontalLine << boardTokens.horizontalLine << RESET;
    }
    std::cout << BOARD_COLOR << colorMatrix.matrix[row][COLUMNS - 1].bottomRightCorner << RESET;
}

void RenderV2::render_middle(int row, Board &board, PlayerColor color) {
    if (color == BIANCO) {
        for (int i = 0; i < COLUMNS; i++) {
            RenderSquare currentColor = colorMatrix.matrix[row][i];
            std::string currentPiece = square_resolve(Coords(ColumnNotation(i), row), board);

            std::cout << currentColor.westColor << boardTokens.verticalLine << RESET;
            std::cout << boardTokens.filling << boardTokens.filling;
            //Note for future (if you want to color pieces) this is where you render pieces
            std::cout << PIECE_COLOR << currentPiece << RESET;
            std::cout << boardTokens.filling << boardTokens.filling;
        }
        std::cout << colorMatrix.matrix[row][COLUMNS - 1].eastColor << boardTokens.verticalLine << RESET;
    } else { // Doing the same thing as in RenderV2::render_top()
        for (int i = 0, colorRow = 7 - row; i < COLUMNS; i++) {
            RenderSquare currentCorner = colorMatrix.matrix[row][i];
            RenderSquare currentColor = colorMatrix.matrix[colorRow][i];

            std::string currentPiece = square_resolve(Coords(ColumnNotation(i), colorRow), board);

            std::cout << currentCorner.westColor << boardTokens.verticalLine << RESET;
            std::cout << boardTokens.filling << boardTokens.filling;
            //Note for future (if you want to color pieces) this is where you render pieces
            std::cout << BOARD_COLOR << currentPiece << RESET;
            std::cout << boardTokens.filling << boardTokens.filling;
        }
        std::cout << colorMatrix.matrix[7 - row][COLUMNS - 1].eastColor << boardTokens.verticalLine << RESET;
    }
}

void RenderV2::render_board(Board &board, PlayerColor color, Move move) {
    // If move.color is TRASPARENTE the move wasn't provided and the board should be white, otherwize color according to the move
    if (move.color != TRASPARENTE) {
        colorMatrix.color_board(move);
    }

    switch(color) {
        case BIANCO:
            render_top(ROWS - 1, BIANCO);
            std::cout << std::endl;
            for (int row = ROWS - 1; row >= 0; row--) {
                render_middle(row, board, BIANCO);
                std::cout << std::endl;
                render_bottom(row);
                std::cout << std::endl;
            }
            break;
        case NERO:
            render_top(ROWS - 1, NERO);
            std::cout << std::endl;
            for (int row = ROWS - 1; row >= 0; row--) {
                render_middle(row, board, NERO);
                std::cout << std::endl;
                render_bottom(row);
                std::cout << std::endl;
            }
    }
}

std::string RenderV2::square_resolve(Coords coords, Board &board) {
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
    int topOffset = colorOffset % 10;
    int bottomOffset = colorOffset / 10;
    bool drawn = false;

    for (int i = 1; i < columns; i++) {
        if (i == topOffset) {
            drawn = true;
            border(boardTokens.link, lineColor);
        } else if (i == bottomOffset) {
            drawn = true;
            border(boardTokens.link, lineColor);
        }
        if (drawn == false) {
            border(boardTokens.link);
        }
        drawn = false;
    }
    // Need to work on this
    border(boardTokens.rightBorder);
    std::cout << std::endl;
}

void StdRender::middle(Board &b, PlayerColor playerColor, int rows, int columns, Color colorOffsets) {
    std::string CURRENT_COLOR = BOARD_COLOR;
    // This is true when a square is being colored
    bool wasColored = false;
    bool shouldColor = false;
    // This is for coloring the top and bottom of a square
    bool topLine = false;
    bool bottomLine = false;
    // This describes how to color two squares, since a line can be colored in 2 segments
    int lineOffset = 0;

    int topLineIndex = 0;
    int bottomLineIndex = 0;
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
                                bottomLine = true;
                                bottomLineIndex = i;
                                // Exclude this index from being colored in the future
                                colorOffsets.moveRows[i] = 9;
                            } else {
                                // This is the line above the one which should be printed, tells the line to color the top segment
                                if (colorOffsets.moveRows[i] == 1) {
                                    topLine = true;
                                    topLineIndex = i;
                                }
                                // moveRows says how many rows you have to count before coloring one
                                // when you get to 0 you are on the row you should color
                                colorOffsets.moveRows[i]--;
                            }
                        }
                        // Print row number
                        std::cout << _row + 1 << "  ";

                        for (int _col = 0; _col < columns; _col++) {
                            if (shouldColor) {
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
                            if (topLine) {
                                lineOffset += colorOffsets.moveColumns[topLineIndex] + 1;
                            }
                            if (bottomLine) {
                                if (colorOffsets.moveColumns[bottomLineIndex] == 0) {
                                    lineOffset += 10;
                                } else {
                                    lineOffset += colorOffsets.moveColumns[bottomLineIndex] * 10;
                                }
                            }
                            if (topLine || bottomLine) {
                                line(columns, lineOffset, MOVE_COLOR);
                                topLine = false;
                                bottomLine = false;
                                lineOffset = 0;
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
                           Player& blackPlayer, GameState result) const {
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
