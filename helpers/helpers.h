#pragma once

#include <iostream>

#define columns 8
#define rows 8

enum Piece {
    VUOTA, DAMA_B, DAMA_N, DAMONE_B, DAMONE_N, COLORATA
};

enum ColumnNotation {
    A = 0, B, C, D, E, F, G, H
};

enum SquareColor {
    BIANCA, NERA
};

enum PlayerColor {
    BIANCO, NERO
};

class Coords {
public:
    ColumnNotation column;
    int row;

    Coords(ColumnNotation _column, int _row)
            : column(_column), row(_row) {}
};

class Square {
public:
    const Coords coords;
    const SquareColor color;
    Piece piece;

    void set_piece(Piece _piece);

    Square(Coords _coords, SquareColor _color, Piece _piece = VUOTA)
            : coords(_coords), color(_color), piece(_piece) {}
};
