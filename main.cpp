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

/*
    // Test Move
    Coords start = Coords(E, 3);
    Coords end = Coords(D, 4);

    Move x = Move(start, end, BIANCO, MOVE);
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
    */
    // ===== TESTING EXECUTE_MOVE()
    // SETTING UP THE BOARD
    Move first(Coords(B, 6), Coords(B, 4), BIANCO, MOVE);
    Move second (Coords(A, 7), Coords(B, 6), NERO, MOVE);

    Move eat (Coords(A, 3), Coords(B, 4), BIANCO, EAT);
    std::vector<Coords> eatenCoords;

    eatenCoords.emplace_back(B, 4);
    eatenCoords.emplace_back(B, 6);
    eat.fill_eat(eatenCoords);
    engine.render.render_board(BIANCO, engine.board);
    engine.board.execute_move(first);
    engine.render.render_board(BIANCO, engine.board);
    engine.board.execute_move(second);
    engine.render.render_board(BIANCO, engine.board);

    engine.board.execute_move(eat);

    // RENDERING
    engine.render.render_board(BIANCO, engine.board);

    engine.check_eat(Move(Coords(B, 8), Coords(A, 7), NERO, EAT));

    //std::cout << engine.count_pieces(BIANCO) << std::endl;
}
