#include "GraphicalDisplay.h"
#include "window.h"
#include "Piece.h"
#include <utility>
#include <string>

void GraphicalDisplay::drawTile(pair<int, int> position) {
    shared_ptr<Piece> piece = grid.getPiece(position);

    int x = position.first * 50;
    int y = 400 - (position.second + 1) * 50;
    std::string label;

    if ((position.first + position.second + 1) % 2) {
        // draw tile underneath
        window.fillRectangle(x, y, 50, 50, Xwindow::DarkOliveGreen4);
        // draw label
        if(position.second == 0){
            label = std::string(1, 'a' + position.first);
            window.drawString(x + 35, y + 45, label, Xwindow::LightYellow2);
        }
        if(position.first == 0){
            label = std::to_string(position.second + 1);
            window.drawString(x + 5, y + 15, label, Xwindow::LightYellow2);
        }
    }
    else {
        window.fillRectangle(x, y, 50, 50, Xwindow::LightYellow2);
        // draw label
        if(position.second == 0){
            label = std::string(1, 'a' + position.first);
            window.drawString(x + 35, y + 45, label, Xwindow::DarkOliveGreen4);
        }
        if(position.first == 0){
            label = std::to_string(position.second + 1);
            window.drawString(x + 5, y + 15, label, Xwindow::DarkOliveGreen4);
        }
    }


    if (!piece) return;

    char piece_char = piece->getId();

    if (piece->getColour() == WHITE) {
        piece_char = toupper(piece_char);
    }

    window.drawString(x + 22, y + 30, {piece_char});
}


GraphicalDisplay::GraphicalDisplay(const Grid& grid) : BaseDisplay(grid), window{400,400} {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            drawTile({i, j});
        }
    }
    prev_state = grid.clone();
}

void GraphicalDisplay::notify() {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            const auto& prev = prev_state.getPiece(i, j);
            const auto& now = grid.getPiece(i, j);

            // prev and now must have the same emptiness/non-emptiness
            if ((!prev == !now)) {
                // if both are empty no need to re draw
                if (!prev) continue;

                // if both are not empty, no need to redraw if they are the same piece and colour
                if (prev->getId() == now->getId() && prev->getColour() == now->getColour()) continue;
            }

            // redraw this tile
            drawTile({i, j});
        }
    }

    prev_state = grid.clone();
}
