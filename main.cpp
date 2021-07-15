#include "render/render.h"
#include "ui/ui.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch"

int main() {
    // Initialize board looks
    BoardTokens board(NORMAL);
    SetPieces pieces(NORMAL);
    BoardCoords coords(NORMAL);
    GameEngine engine(STANDARD, board, pieces, coords, 8, 8);

    PlayerColor currentPlayer = NERO;

    UI::init(engine);

//    engine.render.end_screen(20, 19, engine.whitePlayer, engine.blackPlayer);

    // Main loop
    while (engine.game_over(8, 8) == GOOD) {
        // Switch playerColor every turn
        switch (currentPlayer) {
            case BIANCO:
                currentPlayer = NERO;
                break;
            case NERO:
                currentPlayer = BIANCO;
                break;
        }

        engine.render.render_board(currentPlayer, engine.board, 8, 8);
        Move move(currentPlayer);

        // ========= GETTING INPUT ==========
        // get_move returns MISINPUT if the move provided is very wrong (e.g. A9-KH)
         while (UI::get_move(move, engine, currentPlayer) == MISINPUT) {
         }
         while (engine.submit(move) != VALID) {
             UI::get_move(move, engine, currentPlayer);
         }
         engine.promote(8, 8);
    }
}

#pragma clang diagnostic pop

// Blowing has to be the first thing you do
