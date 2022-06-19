#pragma once

#include "../../move/move.h"

class ApiumLine {
public:
    ApiumLine() = default;

    ApiumLine(std::vector<Move> moves, float eval);

    float get_eval() const;

    void set_eval(float newEval);

    void push_move(const Move& move) noexcept;

    // Deletes the latest move from the moves vector
    void pop_move() noexcept;

    bool operator<(const ApiumLine& rhs) const;

    bool operator>(const ApiumLine &rhs) const;

private:
    std::vector<Move> m_moves;
    float m_eval;
};

