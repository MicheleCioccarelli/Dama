#include "player.h"

void Player::add_move(Move move) {
    m_moves.push_back(move);
}

PlayerColor Player::other_color() {
    if (m_color == BIANCO) {
        return NERO;
    } else if (m_color == NERO){
        return BIANCO;
    }
    return TRASPARENTE;
}

Player &Player::operator=(const Player &rhs) {
    this->m_color = rhs.m_color;
    this->m_moves = rhs.m_moves;
    this->name = rhs.name;
    return *this;
}