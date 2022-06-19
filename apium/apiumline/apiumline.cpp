#include "apiumline.h"

ApiumLine::ApiumLine(std::vector<Move> moves, float eval)
    : m_moves(moves), m_eval(eval) {}

bool ApiumLine::operator<(const ApiumLine &rhs) const {
    return m_eval < rhs.get_eval();
}

bool ApiumLine::operator>(const ApiumLine &rhs) const {
    return m_eval > rhs.get_eval();
}

void ApiumLine::set_eval(float newScore) {
    m_eval = newScore;
}

float ApiumLine::get_eval() const {
    return m_eval;
}

void ApiumLine::push_move(const Move &move) noexcept {
    m_moves.push_back(move);
}

void ApiumLine::pop_move() noexcept {
    m_moves.pop_back();
}