#include "ui.h"

MoveCase UI::command_to_move(const std::vector<Command>& commands, Move &move) {
    for (int i = 0; i < commands.size(); i++) {
        switch (commands[i].type.movetype) {
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
                for (int j = 0; j < commands[i].eatenCoords.size(); j++) {
                    move.coords.push_back(commands[i].eatenCoords[j]);
                }
                i = commands.size();
            case UNINITIALIZED:
                i = commands.size();
        }
        switch (commands[i].type.moveReturn) {
            case  TOO_SHORT:
                move.type = TOO_SHORT;
            case WRONG_OPERATOR:
                move.type = WRONG_OPERATOR;
        }
    }
    return MoveType(VALID);
}

void UI::get_move(Move& move, GameEngine& engine) {
    std::vector<std::string> input;
    std::vector<Command> commands;

    std::string a;
    std::cout << "Move > ";
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

    // Handle error messages here
    command_to_move(commands, move);
}
