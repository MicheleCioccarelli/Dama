#include "ui.h"


Coords UI::convert_coords(char _column, char _row) {
        ColumnNotation column{Z};
        int row{9};

        // Initialize startingColumn
        switch (_column) {
            case 'A':
                column = A;
                break;
            case 'B':
                column = B;
                break;
            case 'C':
                column = C;
                break;
            case 'D':
                column = D;
                break;
            case 'E':
                column = E;
                break;
            case 'F':
                column = F;
                break;
            case 'G':
                column = G;
                break;
            case 'H':
                column = H;
                break;
            default:
                column = Z;
        }
        // Initialize startingRow
        if (_row - 48 < 0) {
            row = 9;
        } else {
            row = (int) _row - 48;
        }
    return Coords(column, row);
}

void UI::get_move() {
    std::string input;
    std::cout << "Halo" << std::endl;
    getline(std::cin, input);
    std::stringstream ss(input);

    while (ss.good()) {
        std::string token;
        ss >> token;
        std::cout << token << std::endl;
    }
}
