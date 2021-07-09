#include "movecase.h"

MoveCase& MoveCase::operator=(const MoveCase &rhs) {
    moveReturn = rhs.moveReturn;
    movetype = rhs.movetype;
    return *this;
}

MoveCase & MoveCase::operator=(const MoveType &rhs) {
    movetype = rhs;
    return *this;
}

MoveCase & MoveCase::operator=(const MoveReturn &rhs) {
    moveReturn = rhs;
    return *this;
}

bool MoveCase::operator==(const MoveType &rhs) {
    if (movetype == rhs) {
        return true;
    }
    return false;
}
