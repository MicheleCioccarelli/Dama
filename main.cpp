#include "render/render.h"
#include "engine/engine.h"

int main() {
    BoardTokens board(NORMAL);
    SetPieces pieces(NORMAL);
    BoardCoords coords(NORMAL);

    GameEngine engine(STANDARD, board, pieces, coords);

    engine.board.empty_piece_initialization();

    Move move(NERO, MOVE);

    engine.board.edit(Coords(H, 8), DAMONE_N);
    engine.board.edit(Coords(G, 7), DAMONE_B);
    engine.render.render_board(BIANCO, engine.board);
    if(engine.check_blow(Coords(G, 7), Coords(H, 8))
        == BLOWABLE) {
        engine.board.blow_up(Coords(G, 7), move);
    }

    engine.render.render_board(BIANCO, engine.board);
}
