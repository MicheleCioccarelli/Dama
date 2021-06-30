#include "renders/render.h"
#include "engine/engine.h"

int main() {
    GameEngine engine(STANDARD);

    BoardTokens stdBoard(NORMAL);

    SetPieces stdPieces(NORMAL);

    BoardCoords stdCoords(NORMAL);

    StdRender stdRender(stdBoard, stdPieces, stdCoords);

    engine.dispatch_move(Move(Coords(F, 6), Coords(F, 4)));
    engine.dispatch_move(Move(Coords(H, 6), Coords(G, 5)));

    engine.check_eat(Move(Coords(G, 3), Coords(F, 4)));

    stdRender.board(BIANCO, engine.board);

    std::cout << engine.count_pieces(BIANCO) << std::endl;
}
