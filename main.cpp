#include "board/board.h"

int main() {
    Board a;

    for (int row = 0; row < rows; row++) {
        std::cout << std::endl;
        for (int i = 0; i < columns; i++) {
            if (a.board[row][i].piece == DAMA_B) {
                std::cout << "●";
            } else if (a.board[row][i].piece == DAMA_N) {
                std::cout << "○";
            } else {
                std::cout << " ";
            }
        }
    }

    std::cout << std::endl;
}
