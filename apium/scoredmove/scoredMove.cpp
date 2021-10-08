#include "scoredMove.h"

bool ScoredMove::operator<(const ScoredMove &rhs) const {
    if (this->score < rhs.score) {
        return true;
    }
        return false;
}

bool ScoredMove::operator>(const ScoredMove &rhs) const {
    if (this->score > rhs.score) {
        return true;
    }
    return false;
}
