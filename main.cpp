#include "gamehandler/gamehandler.h"
#include <cstring>

int main(int argc, char* argv[]) {
    // Command line arguments not really implemented yet
    std::string input {};
    if (argc != 1) {
//        if (strcmp(argv[1], "-apium") != 0) {
//            // The only available argument at the moment is -apium
//            GameHandler::cli_error(WRONG_ARGUMENT);
        /*} else */if (strcmp(argv[1], "-playback") == 0) {
            if (argc <= 2) {
                std::cout << "Quale file vuoi visualizzare?\n>";
                getline(std::cin, input);
                GameHandler::play_back(input);
            } else {
                GameHandler::play_back(argv[2]);
            }
        } else {
            // Play against Apium
            // Remember to keep Apium's game engine up to date
        }
    } else {
        GameEngine engine(STANDARD);

        UI::init(engine);

        GameHandler::two_player_game(engine);
    }
    return 0;
}

// TODO BLOW FAILS AFTER [A3-B4],[H6-G5],[E3-F4],[D6-C5]