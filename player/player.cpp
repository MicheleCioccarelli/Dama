#include "player.h"

void Player::add_move(Move move) {
    moves.push_back(move);
}

PlayerColor Player::other_color() {
    if (color == BIANCO) {
        return NERO;
    } else if (color == NERO){
        return BIANCO;
    }
    return TRASPARENTE;
}

Player &Player::operator=(const Player &rhs) {
    this->color = rhs.color;
    this->moves = rhs.moves;
    this->name = rhs.name;
    return *this;
}