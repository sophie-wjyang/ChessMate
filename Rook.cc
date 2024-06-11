#include "Rook.h"


Rook::Rook(const Grid& grid, const Colour colour, const pair<int, int>& position, const bool has_moved = false)
        : Piece(ROOK, grid, colour, position), has_moved{has_moved} {}


shared_ptr<Piece> Rook::clone(const Grid& grid_clone) const {
    return make_shared<Rook>(grid_clone, colour, position, has_moved);
}


vector<Move> Rook::getVisibleMoves() const {
    vector<Move> moves;

    // moves in any of the four vertical/horizontal directions, any distance, but cannot move past any piece that blocks its path
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            if (x != 0 && y != 0) {
                continue;
            }

            pair<int, int> direction = {x, y};

            // get all the next positions in the direction given
            vector<pair<int, int>> next_positions = scanDirection(direction);

            // check next direction if scan found nothing
            if (next_positions.empty()) continue;

            // get the last tile in the directional scan;
            shared_ptr<Piece> p = grid.getPiece(next_positions.back());

            // remove the last position in the next_positions if it's a piece of the same colour
            if (p && p->getColour() == colour) {
                next_positions.pop_back();
            }

            for (auto next_position: next_positions) {
                Move move = {position, next_position};
                moves.emplace_back(move);
            }
        }
    }

    return moves;
}

bool Rook::getHasMoved() const{
    return has_moved;
}


void Rook::setHasMoved(bool val) {
    has_moved = val;
}
