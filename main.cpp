#include "render/render.h"
#include "ui/ui.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch"

int main() {
    // Initialize board looks
    BoardTokens board(NORMAL);
    SetPieces pieces(NORMAL);
    BoardCoords coords(NORMAL);
    GameEngine engine(STANDARD, board, pieces, coords);

    PlayerColor currentPlayer = NERO;

    // Main loop
    while (!engine.game_over()) {
        // Switch playerColor every turn
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

        // ========= GETTING INPUT ==========
        // get_move returns MISINPUT if the move provided is very wrong (e.g. A9-KH)
         while (UI::get_move(move, engine, currentPlayer) == MISINPUT) {
         }
         while (engine.submit(move) != VALID) {
             UI::get_move(move, engine, currentPlayer);
         }
    }
}

#pragma clang diagnostic pop

// Blowing has to be the first thing you do