#include "renders/render.h"

int main() {
    Board a;

    BoardTokens stdBoard(DEFAULT);

    SetPieces stdPieces(DEFAULT);

    BoardCoords stdCoords(DEFAULT);

    StdRender stdRender(stdBoard, stdPieces, stdCoords);

    a.standard_game_initialization();

    stdRender.board(NERO, a);

    std::cout << std::endl;
}
