#include "gamehandler/gamehandler.h"
#include <cstring>

int main(int argc, char* argv[]) {
    // Command line arguments not really implemented yet
    if (argc != 1) {
        if (argc - 1 > 1) {
            GameHandler::cli_error(TOO_MANY_ARGUMENTS);
        } else if (strcmp(argv[1], "-apium") != 0) {
            // The only available argument at the moment is -apium
            GameHandler::cli_error(WRONG_ARGUMENT);
        } else {
            // Play against Apium
            // Remember to keep Apium's game engine up to date
        }
    } else {
        GameEngine engine(STANDARD);

        UI::init(engine);

        GameHandler::debug(engine);
    }
    return 0;
}
