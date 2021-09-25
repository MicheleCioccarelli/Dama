#include "render/render.h"
#include "ui/ui.h"

int main() {
    BoardTokens board(NORMAL);

    GameEngine engine(STANDARD, NORMAL, NORMAL, NORMAL);

    Player currentPlayer;
    currentPlayer.color = NERO;

    UI::init(engine);
    engine.render.render_board(engine.board, BIANCO);
    // Main loop
    while (engine.game_over() == GOOD) {
        // Switch player every turn, used both for colors and for move logs
        switch (currentPlayer.color) {
            case BIANCO:
                currentPlayer = engine.blackPlayer;
                break;
            case NERO:
                currentPlayer = engine.whitePlayer;
                break;
        }
        Move move(currentPlayer.color);

        // get_move returns MISINPUT if the move provided is very wrong (e.g. A9-KH)
        while (UI::get_move(move, engine, currentPlayer.color) == MISINPUT) {
        }
        while (engine.submit(move) != VALID) {
            UI::get_move(move, engine, currentPlayer.color);
        }

        // See if a piece has gotten to the end, if so promote it
        engine.promote();

        // Renders looks for the last player's newest move for coloring, then switches the board to the opposite side
        engine.render.render_board( engine.board, currentPlayer.color, move);
        engine.render.render_board( engine.board, currentPlayer.other_color(), move);
    }

    // Needs to be re-written
    engine.render.render_board( engine.board, currentPlayer.color);

    int whitePieces = engine.count_pieces(BIANCO);
    int blackPieces = engine.count_pieces(NERO);

    engine.render.end_screen(whitePieces, blackPieces, engine.whitePlayer, engine.blackPlayer,
                             engine.game_over());
}
