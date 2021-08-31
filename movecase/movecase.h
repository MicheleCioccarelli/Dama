#pragma once

#include "../enums/MoveReturn.h"
#include "../enums/MoveType.h"

class MoveCase {
public:
    MoveType moveType;
    MoveReturn moveReturn;

    MoveCase& operator=(const MoveCase &rhs);
    MoveCase& operator=(const MoveReturn &rhs);
    MoveCase& operator=(const MoveType &rhs);

    bool operator==(const MoveType &rhs);

    MoveCase(MoveType _movetype = UNINITIALIZED, MoveReturn _moveReturn = UNDEFINED)
        : moveType(_movetype), moveReturn(_moveReturn) {}
};
