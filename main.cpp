#include "renders/render.h"
#include "engine/engine.h"

int main() {
    BoardTokens board(NORMAL);
    SetPieces pieces(NORMAL);
    BoardCoords coords(NORMAL);

    StdRender render(board, pieces, coords);



    GameEngine engine(STANDARD);

    //engine.dispatch_move(Move(Coords(F, 6), Coords(F, 4)));
    //engine.dispatch_move(Move(Coords(H, 6), Coords(G, 5)));

    engine.board.board[3][D].piece = VUOTA;

    engine.check_eat(Move(Coords(E, 3), Coords(D, 4)));

    render.board(BIANCO, engine.board);

    std::cout << engine.count_pieces(BIANCO) << std::endl;
}

// Put change_type in player
