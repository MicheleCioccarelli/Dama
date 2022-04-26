#include "fileHandler.h"

fileIssue FileHandler::create_file(GameEngine &engine, const std::string& gameName, std::string &filePath, const int gameState) {

    if (filePath.find(gameName) == std::string::npos) {
        // The user didn't already put the game name in the path
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
        // ADD ALL THE MOVES
    }

    o_file.close();
    return FILE_CLOSED_CORRECTELY;
}