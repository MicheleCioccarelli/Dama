#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
#include "render.h"

// RENDER V2
RenderV2::RenderV2() {
    colorMatrix = ColorMatrix();
}

void RenderV2::padding(int numberOfSpaces) {
    for (int i = 0; i < numberOfSpaces; i++) {
        std::cout << " ";
    }
}

void RenderV2::render_top(int row) {
        for (int i = 0; i < COLUMNS; i++) {
            RenderSquare currentSquare = colorMatrix.matrix[row][i];

            std::cout << BOARD_COLOR << currentSquare.topLeftCorner << RESET;

            std::cout << currentSquare.color.northColor << HORIZONTAL_LINE << HORIZONTAL_LINE <<
                      HORIZONTAL_LINE << HORIZONTAL_LINE << HORIZONTAL_LINE << RESET;
    }
    std::cout << BOARD_COLOR << colorMatrix.matrix[row][COLUMNS - 1].topRightCorner << RESET;
}

void RenderV2::render_bottom(int row) {
    for (int i = 0; i < COLUMNS; i++) {
        RenderSquare currentSquare = colorMatrix.matrix[row][i];

        std::cout << BOARD_COLOR << currentSquare.bottomLeftCorner << RESET;

        std::cout << currentSquare.color.southColor << HORIZONTAL_LINE << HORIZONTAL_LINE <<
                  HORIZONTAL_LINE << HORIZONTAL_LINE << HORIZONTAL_LINE << RESET;
    }
    std::cout << BOARD_COLOR << colorMatrix.matrix[row][COLUMNS - 1].bottomRightCorner << RESET;
}

void RenderV2::render_middle(int row, Board &board, PlayerColor color) {
        for (int i = 0; i < COLUMNS; i++) {
            RenderSquare currentColor = colorMatrix.matrix[row][i];
            std::string currentPiece;
            if (color == BIANCO) {
                currentPiece = square_resolve(Coords(ColumnNotation(i), row), board);
            } else {
                currentPiece = square_resolve(Coords(ColumnNotation(7 - i), 7 - row), board);
            }

            std::cout << currentColor.color.westColor << VERTICAL_LINE << RESET;
            std::cout << FILLING << FILLING;
            //Note for future (if you want to color pieces) this is where you render pieces
            std::cout << PIECE_COLOR << currentPiece << RESET;
            std::cout << FILLING << FILLING;
        }
        std::cout << colorMatrix.matrix[row][COLUMNS - 1].color.eastColor << VERTICAL_LINE << RESET;
}

void RenderV2::render_board(Board &board, PlayerColor color, Move move) {
    // If move.color is TRASPARENTE the move wasn't provided and the board should be white, otherwize color according to the move
    if (move.playerColor != TRASPARENTE) {
        colorMatrix.color_board(move);
        if (color == NERO) {
            colorMatrix.flip_board();
        }
    }
    std::cout << "    ";
    render_top(ROWS - 1);
    std::cout << std::endl;

    for (int row = ROWS - 1; row >= 0; row--) {
        render_rows(color, row);
        render_middle(row, board, color);
        std::cout << std::endl;
        std::cout << "    ";
        render_bottom(row);
        std::cout << std::endl;
    }
    render_columns(color);
    std::cout << std::endl;
    clear();
}

void RenderV2::clear() {
    colorMatrix.clear();
}

std::string RenderV2::square_resolve(Coords coords, Board &board) {
    switch (board.matrix[coords.row][coords.column].piece.color) {
        case BIANCO:
            switch (board.matrix[coords.row][coords.column].piece.type) {
                case DAMA:
                    return "●";
                case DAMONE:
                    return "▲";
                case VUOTA:
                    return " ";
                case COLORATA:
                    return "█";
            }
        case NERO:
            switch (board.matrix[coords.row][coords.column].piece.type) {
                case DAMA:
                    return "○";
                case DAMONE:
                    return "Δ";
                case VUOTA:
                    return " ";
                case COLORATA:
                    return "█";
            }
        default:
            return " ";
    }
}

void RenderV2::render_rows(PlayerColor color, int row) {
    if (color == BIANCO) {
        std::cout << BOARD_COLOR << row + 1 << "   " << RESET;
    } else {
        std::cout << BOARD_COLOR << 8 - row << "   " << RESET;
    }
}

void RenderV2::render_columns(PlayerColor color) {
    int k;
    std::cout << "       ";
    for (int i = 0; i < COLUMNS; i++) {
        if (color == NERO) {
            k = 7 - i;
        } else {
            k = i;
        }
        // a ASCII's value is 97
        char currentLetter = 'A' + k;
        std::cout << BOARD_COLOR << currentLetter << RESET;
        std::cout << "     ";
    }
}

void RenderV2::center_name(std::string& name, int longerName, const std::string& color) {
    if (name.size() == longerName) {
        std::cout << color << name << RESET;
    } else {
        int diff = name.size() - longerName;
        if (diff < 0) {
            diff = -diff;
        }

        for (int i = 0; i < diff / 2; i++) {
            std::cout << " ";
        }
        std::cout << color <<  name << RESET;

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

void RenderV2::end_screen(int whitePieces, int blackPieces, Player whitePlayer, Player blackPlayer, GameState result, time_t start) {
     std::vector<std::string> strings;
     strings.emplace_back("Nome");
     strings.emplace_back("Pezzi");
     strings.emplace_back("Mosse");

     std::string WHITE_COLOR = WINNER_COLOR;
     std::string BLACK_COLOR = WINNER_COLOR;

     switch (result) {
         case WHITE_RESIGNED:
             strings.emplace_back("Il bianco si è arreso");
             WHITE_COLOR = LOSER_COLOR;
             break;
         case BLACK_RESIGNED:
             strings.emplace_back("Il nero si è arreso");
             BLACK_COLOR = LOSER_COLOR;
             break;
         case WHITE_WIN:
             strings.emplace_back("Vittoria bianca");
             BLACK_COLOR = LOSER_COLOR;
             break;
         case BLACK_WIN:
             strings.emplace_back("Vittoria nera");
             WHITE_COLOR = LOSER_COLOR;
         case DRAW:
             strings.emplace_back("Pareggio");
             WHITE_COLOR = DRAW_COLOR;
             BLACK_COLOR = DRAW_COLOR;
             break;
         case GAME_NOT_OVER:
             strings.emplace_back("");
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

     std::cout << "\n\n";
     padding(4);
     center_name(strings[3], longerName + 13, ANNOUNCMENT_COLOR);
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
     std::cout << TOP_LEFT;
     for (int i = 0; i < longerName + 2; i++) {
         std::cout << HORIZONTAL_LINE;
     }
     std::cout << UP_CHAIN;
     // Pieces block
     for (int i = 0; i < 6; i ++) {
         std::cout << HORIZONTAL_LINE;
     }
     std::cout << UP_CHAIN;
     // Number of moves
     for (int i = 0; i < 6; i++) {
         std::cout << HORIZONTAL_LINE;
     }
     std::cout << TOP_RIGHT;

     padding(5);
     // White name
     std::cout << std::endl;
     padding(5);
     std::cout << VERTICAL_LINE << " ";
     center_name(whitePlayer.name, longerName, WHITE_COLOR);
     std::cout << " " << VERTICAL_LINE;

     // White number of pieces
     if (whitePieces < 10) {
         std::cout << "   " << whitePieces << "  ";
     } else {
         std::cout << "  " << whitePieces << "  ";
     }

     // White number of moves
     if (whitePlayer.moves.size() <= 9) {
         std::cout << VERTICAL_LINE << "   " << whitePlayer.moves.size() << "  ";
     } else {
         std::cout << VERTICAL_LINE << "  " << whitePlayer.moves.size() << "  ";
     }
     std::cout << VERTICAL_LINE;

     // Middle line
     std::cout << std::endl,
     padding(5);
     std::cout << LEFT_BORDER;

     for (int i = 0 ; i < longerName + 2; i++) {
         std::cout << HORIZONTAL_LINE;
     } std::cout << LINK;
     for (int i = 0 ; i < 6; i++) {
         std::cout << HORIZONTAL_LINE;
     } std::cout << LINK;
     for (int i = 0 ; i < 6; i++) {
         std::cout << HORIZONTAL_LINE;
     } std::cout << RIGHT_BORDER;

     // Black information
     std::cout << std::endl;
     padding(5);
     std::cout << VERTICAL_LINE << " ";
     center_name(blackPlayer.name, longerName, BLACK_COLOR);
     std::cout << " " << VERTICAL_LINE;

     if (blackPieces < 10) {
         std::cout << "   " << blackPieces << "  ";
     } else {
         std::cout << "  " << blackPieces << "  ";
     }

     if (blackPlayer.moves.size() <= 9) {
         std::cout << VERTICAL_LINE << "   " << blackPlayer.moves.size() << "  ";
     } else {
         std::cout << VERTICAL_LINE << "  " << blackPlayer.moves.size() << "  ";
     }
     std::cout << VERTICAL_LINE << std::endl;

     padding(5);
     // Closing line
     std::cout << BOTTOM_LEFT;
     for (int i = 0; i < longerName + 2; i++) {
         std::cout << HORIZONTAL_LINE;
     }
     std::cout << DOWN_CHAIN;
     // Pieces block
     for (int i = 0; i < 6; i ++) {
         std::cout << HORIZONTAL_LINE;
     }
     std::cout << DOWN_CHAIN;
     // Number of moves
     for (int i = 0; i < 6; i++) {
         std::cout << HORIZONTAL_LINE;
     }
     std::cout << BOTTOM_RIGHT << std::endl;

    // Print out the time elapsed since the start of the game
    std::cout << std::endl << "Tempo passato: ";
    int minutesElapsed = difftime( time(nullptr), start) / 60;
    double secondsElapsed = difftime( time(nullptr), start);
    if (minutesElapsed > 0) {
        std::cout << minutesElapsed << "m";
        secondsElapsed -= minutesElapsed*60;
    }
    std::cout << secondsElapsed << "s" << std::endl;
}

void RenderV2::cli_help_page() {
    std::cout << "\nComandi:" << std::endl;
    std::cout << "-apium";
    padding(10);
    std::cout << "| Gioca contro Apium" << std::endl;
    std::cout << "Nessun comando";
    padding(2);
    std::cout << "| Gioca contro un'altra persona" << std::endl;
}

void RenderV2::help_page() {
    std::cout << std::endl << std::endl << std::endl;
    for (int i = 1; i < 22; i++) {
        std::cout << "════";
    }
    std::cout << std::endl << CYN <<"Pagina di aiuto" << RESET << std::endl << std::endl;
    std::cout << "Sintassi: " << MAG << "[colonna]" << CYN <<"[riga]" << RESET << "[operatore]" << MAG <<
    "[colonna]" << CYN << "[riga]" << RESET << std::endl << std::endl;
    std::cout << "Per esempio per muoversi da A3 a B4: " << MAG <<"A" << CYN "3" << RESET "-" << MAG << "B" << CYN << "4" << RESET << std::endl;

    std::cout << std::endl << "Colonne: " << MAG << "A, B, C, D, E, F, G, H" << RESET << std::endl;
    std::cout << "Righe:   " << CYN << "1, 2, 3, 4, 5, 6, 7, 8" << RESET << std::endl << std::endl;

    std::cout << "Operatori:" << std::endl;

    padding(3);
    std::cout << MOVE_COLOR << "-" << RESET;
    padding(3);
    std::cout << "Movimento, cambia casella di 1 alla volta";
    padding(8);
    std::cout << "|  Sintassi: A3" << MOVE_COLOR << "-" << RESET << "B4" << std::endl;

    padding(3);
    std::cout << EAT_COLOR << "x" << RESET;
    padding(3);
    std::cout << "Mangiare";
    padding(41);
    std::cout << "|  Sintassi: A3" << EAT_COLOR "x" << RESET << "B4" << std::endl;

    padding(3);
    std::cout << BLOW_COLOR << "*" << RESET;
    padding(3);
    std::cout << "Soffio, deve essere separato da altri comandi";
    padding(4);
    std::cout << "|  Sintassi: A3" << BLOW_COLOR << "*" << RESET << "B4_C3" << MOVE_COLOR << "-" << RESET << "D4" << std::endl;
    padding(7);
    std::cout << "con '_'. Per usarlo bisogna scrivere la mossa";
    padding(4);
    std::cout << "|  Esempio: il nemico non ha fatto A3" << EAT_COLOR << "x" << RESET << "B4," << std::endl;
    padding(7);
    std::cout << "che il nemico non ha fatto, con un " << BLOW_COLOR << "*" << RESET;
    padding(13);
    std::cout << "|  quindi si soffia con A3" << BLOW_COLOR << "*" << RESET << "B4" << std::endl;
    padding(7);
    std::cout << std::endl;

    std::cout << "Comandi:" << std::endl;
    padding(3);
    std::cout << COMMAND_COLOR << "Resign" << RESET << ":  il giocatore che scrive \"resign\" si arrende";
    std::cout << std::endl;
    padding(3);
    std::cout << COMMAND_COLOR << "Summary" << RESET << ": fornisce i dettagli della partita";
    std::cout << std::endl;
    padding(3);
    std::cout << COMMAND_COLOR << "Draw" << RESET << ":    si offre un pareggio";
    std::cout << std::endl;


    for (int i = 1; i < 22; i++) {
        std::cout << "════";
    } std::cout << std::endl;
}
