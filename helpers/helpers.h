#pragma once
#include <iostream>

enum Piece {
    VUOTA, DAMA_B, DAMA_N, DAMONE_B, DAMONE_N
};

enum ColumnNotation {
    A = 0, B, C, D, E, F, G, H, Z
};

enum SquareColor {
    BIANCA, NERA, TRASPARENTE
};

enum PlayerColor {
    BIANCO, NERO
};

class Coords {
public:
    ColumnNotation column;
    int row;
    Coords()
        : column(Z), row(0) {}
    Coords(ColumnNotation _column, int _row)
            : column(_column), row(_row) {}
};

class Square {
public:
    const Coords coords;
    const SquareColor color;
    Piece piece;

    void set_piece(Piece _piece);

    Square()
            : coords(Coords(Z, 0)), color(TRASPARENTE), piece(VUOTA) {}

    Square(Coords _coords, SquareColor _color, Piece _piece = VUOTA)
            : coords(_coords), color(_color), piece(_piece) {}
};
