#pragma once

#include "../board/board.h"
#include "../player/player.h"
#include "../enums/GameStyle.h"
#include "../enums/GameState.h"
#include "../helpPages/helpPages.h"
#include "../enums/MoveIssue.h"

class GameEngine {
public:
    time_t start;
    Player whitePlayer;
    Player blackPlayer;
    Board board;
    RenderV2 render;
    // The boardPos of all the past positions is stored here, used for detecting move repetitionù
    // It is updated by submit()
    std::vector<std::string> pastPositions;

    // Vectors of all the piece-containing squares, used to find them quicker in Apium's minimax()
    std::vector<Square> whitePiecesSquares;
    std::vector<Square> blackPiecesSquares;

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

    // Given a move of EAT type, checks if it can be executed (bounds checking, piece compatibility, ...)
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

    // Does all the checking regarding the move and logs erros if needed
    MoveIssue submit(const Move& move, PlayerColor color);

    // Promotes a piece if the move puts it on the edge of the board
    void precise_promote(const Move& move);

    // Updates whitePieces and blackPieces with information from the current board TODO test if this works
    void refresh_piece_vectors() noexcept;

    void init_piece_vectors();

    std::string get_player_name(PlayerColor color) const;

    /**
     * Checks if a move can be executed, if not returns the reason why
     * */
    MoveIssue check_move(Move& move);

    // Given a move returns the color of the player who made it
    PlayerColor deduce_color_human_notation(const Move &move);

    PlayerColor deduce_color_matrix_notation(const Move &move);

    /**
     * The move you pass in is what the opponent should have eaten, if it could have been done and
     * the enemy didn't eat on their last turn the piece that should have committed murder gets deleted
     * */
    MoveIssue check_blow(Move& move);

    // Adds a move to the players' list of moves
    void add_move(const Move& move);

    int count_pieces(PlayerColor pColor) const;

    // Look if the boardPos passed in is already present in pastPositions<> and return how many times
    int look_for_position(const std::string& beingSearched) const;

    // The color passed in wins, used when resigning. By default checks how many moves are available,
    // if nothing can be done the game is over
    GameState game_over();

    /**
     * Returns the vector of all the moves the piece in matrix[coords] could make, if there are concatenated moves it goes
     * for as long as it can eat pieces, the different vectors span from different turns it could take.
     * Works for both damine and damoni
     * */
    std::vector<Move> simulate_piece(Coords coords);

    /**
     *  Calculate if a piece can move in any of the four directions and returns the resulting moves in a vector
     */
    std::vector<Move> simulate_move_piece(Coords& coords);

    /**
     * Fills the vector with all the possible eat chains starting from coords, returns false if none are available
     * */
    void simulate_eat_piece(std::vector<Move>& movesFound, Coords startingCoords, int index = -1);

    GameEngine(GameStyle gameStyle = STANDARD);
};
