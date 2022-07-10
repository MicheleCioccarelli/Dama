#include "gamehandler/gamehandler.h"
#include <cstring>



// TODO: Always update Apium's m_eval


int main(int argc, char* argv[]) {
    // Command line arguments not really implemented yet
    std::string input{};
    if (argc != 1) {
//        if (strcmp(argv[1], "-apium") != 0) {
//            // The only available argument at the moment is -apium
//            GameHandler::cli_error(WRONG_ARGUMENT);
        /*} else */if (strcmp(argv[1], "-playback") == 0) {
            if (argc <= 2) {
                std::cout << MAG << "Benvenuto nel visualizzatore di partite\n" << RESET;
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
        GameEngine engine(EMPTY);

        UI::init(engine);

        GameHandler::debug(engine);
    }
    return 0;
}
