#include "render/render.h"
#include "engine/engine.h"

int main() {
    BoardTokens board(NORMAL);
    SetPieces pieces(NORMAL);
    BoardCoords coords(NORMAL);

    GameEngine engine(STANDARD, board, pieces, coords);

    engine.board.standard_game_initialization();

    Move first (NERO, MOVE);
    first.add_coords(Coords(B, 6));
    first.add_coords(Coords(B, 4));
    engine.board.execute_move(first);
    engine.render.render_board(BIANCO, engine.board);

    Move eat (BIANCO, EAT);
    eat.add_coords(Coords(A, 3));
    eat.add_coords(Coords(B, 4));

    engine.submit(eat);
    // RENDERING
    engine.render.render_board(BIANCO, engine.board);

    //engine.check_eat(Move(Coords(B, 8), Coords(A, 7), NERO, EAT));

    //std::cout << engine.count_pieces(BIANCO) << std::endl;
}
