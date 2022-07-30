#include "apiumline.h"

#include <utility>

ApiumLine::ApiumLine(std::vector<Move> moves, float eval)
    : m_moves(std::move(moves)), m_eval(eval) {}

ApiumLine::ApiumLine(float eval)
    : m_eval(eval) {}

ApiumLine::ApiumLine(Move move, float eval)
    : m_eval(eval) {
    m_moves.push_back(move);
}

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

std::vector<Move> ApiumLine::get_moves() const {
    return m_moves;
}

void ApiumLine::set_moves(const std::vector<Move>& moves) {
    m_moves = moves;
}

void ApiumLine::push_move(const Move &move) noexcept {
    m_moves.push_back(move);
}

void ApiumLine::pop_move() noexcept {
    m_moves.pop_back();
}