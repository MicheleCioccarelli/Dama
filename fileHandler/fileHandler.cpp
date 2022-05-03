#include "fileHandler.h"

fileIssue FileHandler::create_file(GameEngine &engine, const std::string& gameName, std::string &filePath, const int gameState) {

    if (filePath.find(gameName) == std::string::npos) {
        // The user didn't already put the game name in the path
        filePath.append("/");
        filePath.append(gameName);
    }

    // If the file path/name did not have the right extention, add it
    if (filePath.find(".dama") == std::string::npos) {
        filePath.append(".dama");
    }

    // You can provide the file path. If you just say the name of the game it will be created in the working directory
    std::ofstream o_file {filePath};

    if (!o_file) {
        // Something went wrong
        return OPENING_ISSUE;
    } else {
        o_file << "@BLACK:" << engine.get_player_name(NERO) << "\n";
        o_file << "@BLACK:" << engine.get_player_name(BIANCO) << "\n";
        o_file << "@NAME:" << gameName << "\n";
        o_file << "@STATE:" << gameState << "\n";
        o_file << "@MOVES:";
        // ADD ALL THE MOVES
        std::string temp {};
        int movesToPrint = engine.whitePlayer.moves.size() + engine.blackPlayer.moves.size();
        for (int i = 0; i < engine.whitePlayer.moves.size(); i++) {
            temp = "";
            temp += engine.whitePlayer.moves[i].toString();
            o_file << "[" << temp << "]";

            movesToPrint--;

            if (movesToPrint > 0) {
                o_file << ",";
            }
            temp = "";
            if (engine.blackPlayer.moves.size() > i) {
                temp += engine.blackPlayer.moves[i].toString();
                o_file << "[" << temp << "]";

                movesToPrint--;

                if (movesToPrint > 0) {
                    std::cout << ",";
                }
            }
        }
    }



    // CHECK BETTER BLOW MOVES AND REPETITION

    o_file.close();
    return FILE_CLOSED_CORRECTELY;
}