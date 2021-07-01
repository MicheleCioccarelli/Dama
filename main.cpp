#include "render/render.h"
#include "engine/engine.h"

int main() {
    BoardTokens board(NORMAL);
    SetPieces pieces(NORMAL);
    BoardCoords coords(NORMAL);

    GameEngine engine(STANDARD, board, pieces, coords);


    //engine.dispatch_move(Move(Coords(F, 6), Coords(F, 4)));
    //engine.dispatch_move(Move(Coords(H, 6), Coords(G, 5)));

    // Testing specific scenarios
    engine.board.matrix[3][D].piece = VUOTA;


    // Test Move
    Coords start = Coords(E, 3);
    Coords end = Coords(D, 4);

    Move x = Move(start, end, BIANCO);
    //engine.check_eat(x);
    engine.submit(x);





    Coords start2 = Coords(B, 6);
    Coords end2 = Coords(C, 5);

    Move y = Move(start2, end2, NERO, MOVE);
    //engine.check_eat(x);
    engine.submit(y);

    Coords start3 = Coords(D, 4);
    Coords end3 = Coords(B, 6);

    Move z = Move(start3, end3, NERO, EAT);
    //engine.check_eat(x);
    engine.submit(z);

    // Test Render
    engine.render.render_board(BIANCO, engine.board);

    std::cout << engine.count_pieces(BIANCO) << std::endl;
}
