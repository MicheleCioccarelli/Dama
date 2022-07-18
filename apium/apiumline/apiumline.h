#pragma once

#include "../../move/move.h"

class ApiumLine {
public:
    ApiumLine() = default;

    ApiumLine(float eval);

    ApiumLine(std::vector<Move> moves, float eval);

    float get_eval() const;

    // Returns a vector containinf m_moves
    std::vector<Move> get_moves() const;

    void set_eval(float newEval);

    void set_moves(const std::vector<Move>& moves);

    void push_move(const Move& move) noexcept;

    // Deletes the latest move from the moves vector
    void pop_move() noexcept;

    bool operator<(const ApiumLine& rhs) const;

    bool operator>(const ApiumLine &rhs) const;

private:
    std::vector<Move> m_moves;
    float m_eval = 0;
};

