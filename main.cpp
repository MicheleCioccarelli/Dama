#include "render/render.h"
#include "ui/ui.h"

int main() {
    BoardTokens board(NORMAL);
    SetPieces pieces(NORMAL);
    BoardCoords coords(NORMAL);
    GameEngine engine(STANDARD, board, pieces, coords);

    engine.board.standard_game_initialization();
    engine.board.edit(Coords(B, 4), DAMA_N);

    std::string input = "A3xB4xC5";

    Move move(BIANCO);

    UI::get_move(move, engine);
    std::cout << "hallo" ;
}
