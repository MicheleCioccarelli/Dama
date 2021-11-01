#include "gamehandler/gamehandler.h"
#include <string.h>

int main(int argc, char* argv[]) {

    if (argc != 1) {
        if (argc - 1 > 1) {
            GameHandler::cli_error(TOO_MANY_ARGUMENTS);
        } else if (strcmp(argv[1], "-apium") != 0) {
            GameHandler::cli_error(WRONG_ARGUMENT);
        } else {
            // Play against Apium
        }
    } else {
        GameEngine engine(STANDARD);

        UI::init(engine);

        GameHandler::two_player_game(engine);
    }
    return 0;
}
