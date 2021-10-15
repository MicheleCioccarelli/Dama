#include "render/render.h"
#include "ui/ui.h"

int main() {
    BoardTokens board(NORMAL);

    GameEngine engine(DAMONI, NORMAL, NORMAL, NORMAL);

    Player currentPlayer;
    currentPlayer.color = NERO;

    UI::init(engine);

    engine.render.render_board(engine.board, BIANCO);

    // Main loop
    while (engine.game_over() == GAME_NOT_OVER) {
        // Switch player every turn, used both for colors and for move logs
        switch (currentPlayer.color) {
            case BIANCO:
                currentPlayer = engine.blackPlayer;
                break;
            case NERO:
                currentPlayer = engine.whitePlayer;
                break;
        }
        // Switch player colors
        Move move(currentPlayer.color);

        // FOR TESTING PURPOSES
        engine.render.render_board(engine.board, BIANCO);

        // Get player input
        UI::get_move(move, engine, currentPlayer.color);

        // If the move was invalid/was a command ask for another move
        while (engine.submit(move) != VALID) {
            if (move.type.moveReturn == WHITE_RESIGN || move.type.moveReturn == BLACK_RESIGN) {
                engine.resign(move);
                return 1;
            }
            UI::get_move(move, engine, currentPlayer.color);
        }

        // See if a piece has gotten to the end, if so promote it
        engine.promote();

        // Renders looks for the last player's newest move for coloring, then switches the board to the opposite side
        engine.render.render_board( engine.board, currentPlayer.color, move);
        engine.render.render_board( engine.board, currentPlayer.other_color(), move);
    }

    // When the game is over
    engine.render.render_board( engine.board, currentPlayer.color);

    int whitePieces = engine.count_pieces(BIANCO);
    int blackPieces = engine.count_pieces(NERO);
    engine.render.end_screen(whitePieces, blackPieces, engine.whitePlayer, engine.blackPlayer, engine.game_over(), engine.start);

    return 0;
}
