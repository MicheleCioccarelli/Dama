#include "scoredMove.h"

bool ScoredMove::operator<(const ScoredMove &rhs) const {
    if (m_score < rhs.get_score()) {
        return true;
    }
        return false;
}

bool ScoredMove::operator>(const ScoredMove &rhs) const {
    if (m_score > rhs.get_score()) {
        return true;
    }
    return false;
}

void ScoredMove::set_score(float newScore) {
    m_score = newScore;
}

float ScoredMove::get_score() const {
    return m_score;
}