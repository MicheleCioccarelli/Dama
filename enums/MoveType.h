#pragma once

 enum MoveType {
    MOVE,
    EAT,
    // If a move is uninitialized, it has a problem which is described in MoveReturn,
    // a move can be valid and have MoveReturn BLOWABLE
    UNINITIALIZED,
};

