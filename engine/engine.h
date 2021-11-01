#pragma once

#include "../board/board.h"
#include "../player/player.h"
#include "../enums/GameStyle.h"
#include "../enums/GameState.h"
#include "../render/render.h"

class GameEngine {
public:
    time_t start;
    Player whitePlayer;
    Player blackPlayer;
    Board board;

    // Does all the checking regarding the move and logs erros if needed
    MoveReturn submit(const Move& move);

    // This tests if a move between the first and last element of coords can be performed,
    // the move's type needs to be accurate
    MoveReturn check_move(Move& move);

    // Given a move returns the color of the player who made it
    PlayerColor deduce_color(Move &move);

    // Handles command execution (help page, resigning, summary)
    void execute_command(MoveReturn command);

    MoveReturn check_eat(Move& move);

    // You have to pass in the input flagged as blowable by the user, this function tells
    // you wheter it can be blown, if yes you should call Board::blow_up();
    MoveReturn check_blow(Coords _startingCoords, Coords _endingCoords);

    // Adds the move to the respective player's log and executes it
    void dispatch_move(const Move& move, bool isBlown);
    RenderV2 render;

    int count_pieces(PlayerColor pColor);

    void promote();

    // End the game
    void resign(Move& move);

    // The color passed in wins, used when resigning. By default checks how many moves are available,
    // if nothing can be done the game is over
    GameState game_over(PlayerColor winner = TRASPARENTE);

    // Given a certain piece, looks for all possible moves and returns them in a vector
    std::vector<Move> simulate_damina(Piece piece, Coords coords);
    // Returns a vectors with all the moves the damona could make
    std::vector<Move> simulate_damona(Piece piece, Coords coords);

    GameEngine(GameStyle gameStyle = STANDARD);
};
