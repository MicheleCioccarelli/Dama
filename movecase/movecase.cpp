#include "movecase.h"

MoveCase& MoveCase::operator=(const MoveCase &rhs) {
    moveReturn = rhs.moveReturn;
    moveType = rhs.moveType;
    return *this;
}

MoveCase & MoveCase::operator=(const MoveType &rhs) {
    moveType = rhs;
    return *this;
}

MoveCase & MoveCase::operator=(const MoveReturn &rhs) {
    moveReturn = rhs;
    return *this;
}

bool MoveCase::operator==(const MoveType &rhs) {
    if (moveType == rhs) {
        return true;
    }
    return false;
}
