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

    engine.render.render_board(BIANCO, engine.board);

    Move move(BIANCO);
    UI::get_move(move, engine);

    MoveReturn returnValue = engine.check_eat(move);

    std::cout << "hallo" << std::endl;
}
