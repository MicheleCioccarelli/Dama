#include "ui.h"

MoveType UI::command_to_move(const std::vector<Command>& commands, Move &move) {
    for (int i = 0; i < commands.size(); i++) {
        switch (commands[i].type) {
            case BLOW:
                move.blownCoord = commands[i].startingCoords;
                break;
            case MOVE:
                move.type = MOVE;
                move.add_coords(commands[i].startingCoords);
                move.add_coords(commands[i].endingCoords);
                i = commands.size();
                break;
            case EAT:
                move.type = EAT;
                i = commands.size();
                for (int j = 0; j < commands[i].eatenCoords.size(); j++) {
                    move.coords.push_back(commands[i].eatenCoords[j]);
                }
            case UNINITIALIZED:
                i = commands.size();
            case  TOO_SHORT:
                return TOO_SHORT;
            case WRONG_OPERATOR:
                return WRONG_OPERATOR;
        }
    }
    return OK;
}

void UI::get_move(Move& move, GameEngine& engine) {
    std::vector<std::string> input;
    std::vector<Command> commands;

    std::string a;
    std::cout << "Move" << std::endl;
    getline(std::cin, a);
    std::stringstream stream(a);

    // Initializing the strings
    input.emplace_back("~");
    input.emplace_back("~");

    for (int i = 0; i < MAX_COMMANDS; i++) {
        stream >> input[i];
        std::cout << input[i] << std::endl;

        if (input[i].size() < 5 && input[i] != "~") {
            move.type = TOO_SHORT;
            return;
        }
        commands.emplace_back(input[i], engine);
    }

    command_to_move(commands, move);
}
