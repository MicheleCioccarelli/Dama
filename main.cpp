#include "renders/render.h"
#include "engine/engine.h"

int main() {
    GameEngine engine(DAMONI);

    BoardTokens stdBoard(NORMAL);

    SetPieces stdPieces(NORMAL);

    BoardCoords stdCoords(NORMAL);

    StdRender stdRender(stdBoard, stdPieces, stdCoords);

    engine.dispatch_move(Move(Coords(C, 3), Coords(D, 4)));

    engine.validate_move(Move(Coords(D, 4), Coords(C, 3)));

    std::cout << engine.count_pieces(BIANCO) << std::endl;
}
