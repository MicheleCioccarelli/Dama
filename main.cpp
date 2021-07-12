#include "render/render.h"
#include "ui/ui.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch"

int main() {
    BoardTokens board(NORMAL);
    SetPieces pieces(NORMAL);
    BoardCoords coords(NORMAL);
    GameEngine engine(STANDARD, board, pieces, coords);

    PlayerColor currentPlayer = NERO;

    while (!engine.game_over()) {
        switch (currentPlayer) {
            case BIANCO:
                currentPlayer = NERO;
                break;
            case NERO:
                currentPlayer = BIANCO;
                break;
        }

        engine.render.render_board(currentPlayer, engine.board);
        Move move(currentPlayer);

        UI::get_move(move, engine, currentPlayer);
        while (engine.submit(move) != VALID) {
            UI::get_move(move, engine, currentPlayer);
        }
        engine.submit(move);
    }
}

#pragma clang diagnostic pop