#include "TextDisplay.h"
#include "PieceId.h"
#include <iostream>
#include <string>
#include <memory>

TextDisplay::TextDisplay(const Grid& grid) : BaseDisplay(grid) {}

void TextDisplay::notify() {
    for (int y = 7; y >= 0; y--) { // Starting from 8 to 1
        std::cout << y + 1 << " ";
        for (int x = 0; x < 8; x++) { // For each column in a row
            const auto& piece = grid.getPiece(x, y);
            if (piece) { // If piece exists
                char pieceChar = piece->getId(); // Obtain character ID of the piece
                // If the piece is black, convert the character to lowercase
                if (piece->getColour() == WHITE) {
                    pieceChar = toupper(pieceChar);
                }
                cout << pieceChar;
            }
            else {
                //empty square
                (x + y + 1) % 2 == 0 ? std::cout << " " : std::cout << "_";
            }
        }
        std::cout << std::endl;
    }
    std::cout << "  abcdefgh" << std::endl;
}

