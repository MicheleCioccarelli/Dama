#include "render/render.h"
#include "ui/ui.h"

int main() {
    // Initialize board looks
    BoardTokens board(NORMAL);
    SetPieces pieces(NORMAL);
    BoardCoords coords(NORMAL);
    GameEngine engine(EMPTY, board, pieces, coords, 8, 8);

    PlayerColor currentPlayer = NERO;

    engine.board.edit(Coords(F, 6), DAMONE_B);
    //engine.board.edit(Coords(B, 2), DAMA_B);
    engine.board.edit(Coords(E, 7), DAMONE_N);
    //engine.board.edit(Coords(G, 7), DAMONE_N);


    UI::init(engine);

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

    engine.render.render_board(BIANCO, engine.board, 8, 8);

    int whitePieces = engine.count_pieces(BIANCO, 8, 8);
    int blackPieces = engine.count_pieces(NERO, 8, 8);

    engine.render.end_screen(whitePieces, blackPieces, engine.whitePlayer, engine.blackPlayer,
                             engine.game_over(8, 8));
}
