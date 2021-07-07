#include "render/render.h"
#include "ui/ui.h"

int main() {
    BoardTokens board(NORMAL);
    SetPieces pieces(NORMAL);
    BoardCoords coords(NORMAL);
    GameEngine engine(STANDARD, board, pieces, coords);
    engine.board.standard_game_initialization();

    std::string input = "mm";
    UI::get_move();
}
