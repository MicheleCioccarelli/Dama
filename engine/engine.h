#pragma once

#include "../board/board.h"
#include "../player/player.h"
#include "../enums/GameStyle.h"
#include "../enums/GameState.h"
#include "../render/render.h"
#include "../enums/MoveIssue.h"

class GameEngine {
public:
    time_t start;
    Player whitePlayer;
    Player blackPlayer;
    Board board;

    /**
     * If goingBackwards is true a depth-number of moves will be played backwards. If depth is 1 startingPlayer's last
     * move will be played, if it is 2 also the opposite player's last move will be repeated an so on.
     * If goingBackwards is false you want to get back to the original board position after having gone back, depth should be the same.
     * Returns false if depth is bigger than the moves vector in one of the players
     * */
    bool time_travel(PlayerColor startingPlayer, int depth = 1, bool goingBackwards = true);

    /**
     * The recursive part of time_travel(), playes the moves backwards. The indexes
     * are used to navigate currentPlayer.moves[] because you have to switch player at every iteration
     * */
    bool playBack(PlayerColor currentPlayer, int depth, int whiteIndex, int blackIndex);

    void undo_move(const Move& move);

    MoveIssue recursive_check_eat(Move move, Coords startingCoords = Coords(), int index = 0);

    // Check all the general parameters for an EAT type move (squares must not be white, must move by 1, ...)
    // For more info on the dirt see recursive_check_eat()
    MoveIssue inspect_dama(Coords startingCoords, Coords endingCoords, bool dirt = false);

    // Determines if a coordinate isn't out of the board
    static bool is_in_bounds(Coords coords);

    // Given two squares calculates forwardSquare's coords, see GameEngine::check_eat() to see its main use
    static Coords calculate_forward(const Coords& startingCoords, const Coords& endingCoords);

    // Given an eat type move calculates where the damina will end up in the end
    static Coords calculate_forward(const Move& move);

    // Does all the checking regarding the move and logs erros if needed TODO Fix isBlown
    MoveIssue submit(Move& move);

    /**
     * Checks if a move can be executed, if not returns the reason why
     * */
    MoveIssue check_move(Move& move);

    // Given a move returns the color of the player who made it
    PlayerColor deduce_color(Move &move);

    /**
     * Handles command execution (help page, resigning, summary).
     * Returns true if the game should be ended and changes gameResult to tell game_over() what to do
     * */
    bool execute_command(MoveData command) const;
    /**
     * The move you pass in is what the opponent should have eaten, if it could have been done and
     * the enemy didn't eat on their last turn the piece that should have committed murder gets deleted
     * */
    MoveIssue check_blow(Move& move);

    // Adds the move to the respective player's log and executes it
    void dispatch_move(const Move& move);
    RenderV2 render;

    int count_pieces(PlayerColor pColor) const;

    void promote();

    // The color passed in wins, used when resigning. By default checks how many moves are available,
    // if nothing can be done the game is over
    GameState game_over();

    // Given a certain piece, looks for all possible moves and returns them in a vector
    std::vector<Move> simulate_damina(PlayerColor daminaColor, Coords coords);
    // Returns a vectors with all the moves the damona could make
    std::vector<Move> simulate_damona(Coords coords);
    /** Used by simulate damina, given a direction (offsets) and starting position calculates if
     * it can move/eat in that direction and returns what it can do. It does calculations based on the piece
     * in board at the coordinates + offsets
     */
    std::vector<Move> branch_damina(Coords startingCoords, PlayerColor color, int verticalOffset, int horizontalOffset);

    explicit GameEngine(GameStyle gameStyle = STANDARD);
};
