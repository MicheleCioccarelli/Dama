#include "render/render.h"
#include "ui/ui.h"

int main() {
    BoardTokens board(NORMAL);
    SetPieces pieces(NORMAL);
    BoardCoords coords(NORMAL);

    GameEngine engine(STANDARD, NORMAL, NORMAL, NORMAL);

    RenderV2 render(NORMAL, NORMAL, NORMAL);

    Move hope(BIANCO, MOVE);

    hope.add_coords(Coords(A, 3));
    hope.add_coords(Coords(B, 4));

    render.render_board(engine.board, NERO, hope);

    render.help_page();

/*
// Initialize board looks
    BoardTokens board(NORMAL);
    SetPieces pieces(NORMAL);
    BoardCoords coords(NORMAL);
    GameEngine engine(STANDARD, board, pieces, coords);

    Player currentPlayer;
    currentPlayer.color = NERO;

    UI::init(engine);
    // Main loop
    while (engine.game_over() == GOOD) {
        // Switch playerColor every turn
        switch (currentPlayer.color) {
            case BIANCO:
                currentPlayer = engine.blackPlayer;
                break;
            case NERO:
                currentPlayer = engine.whitePlayer;
                break;
        }
        // Renders looks for the last player's newest move for coloring, then switches the
        // board to the opposite side
        engine.render.render_board(currentPlayer, engine.board, 8, 8);
        Move move(currentPlayer.color);

        // ========= GETTING INPUT ==========
        // get_move returns MISINPUT if the move provided is very wrong (e.g. A9-KH)
        while (UI::get_move(move, engine, currentPlayer.color) == MISINPUT) {
        }
        while (engine.submit(move) != VALID) {
            UI::get_move(move, engine, currentPlayer.color);
        }

        // Sync the player user in render and the one used in engine
        if (currentPlayer.color == BIANCO) {
            currentPlayer = engine.whitePlayer;
        } else {
            currentPlayer = engine.blackPlayer;
        }

        engine.promote();
        engine.render.render_board(currentPlayer, engine.board, 8, 8);
    }

    engine.render.render_board(currentPlayer, engine.board, 8, 8);

    int whitePieces = engine.count_pieces(BIANCO);
    int blackPieces = engine.count_pieces(NERO);

    engine.render.end_screen(whitePieces, blackPieces, engine.whitePlayer, engine.blackPlayer,
                             engine.game_over());*/
}
