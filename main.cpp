#include "render/render.h"
#include "engine/engine.h"

int main() {
    BoardTokens board(NORMAL);
    SetPieces pieces(NORMAL);
    BoardCoords coords(NORMAL);

    GameEngine engine(STANDARD, board, pieces, coords);

    engine.board.standard_game_initialization();

    Move first (BIANCO, MOVE);
    first.add_coords(Coords(C, 3));
    first.add_coords(Coords(C, 5));
    engine.board.execute_move(first);
/*
    Move second (NERO, MOVE);
    second.add_coords(Coords(F, 6));
    second.add_coords(Coords(E, 5));
    engine.board.execute_move(second);

    Move third (NERO, MOVE);
    third.add_coords(Coords(A, 7));
    third.add_coords(Coords(A, 1));
    engine.board.execute_move(third);
*/
    engine.render.render_board(BIANCO, engine.board);
    Move blow(BIANCO, BLOW);
    blow.add_coords(Coords(D, 6));
    blow.add_coords(Coords(C, 5));
/*
    Move eat(BIANCO, EAT);
    eat.add_coords(Coords(B, 6));
    eat.add_coords(Coords(B, 8));
    engine.submit(eat);
*/
    if (engine.check_blow(blow) == BLOWABLE)
        std::cout << "IEMOOO" << std::endl;
    else
        std::cout << "Uffa" << std::endl;

    engine.render.render_board(BIANCO, engine.board);
}
