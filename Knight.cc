#include "Knight.h"
#include <cmath>


Knight::Knight(const Grid& grid, Colour colour, const pair<int, int>& position)
        : Piece(KNIGHT, grid, colour, position) {}


shared_ptr<Piece> Knight::clone(const Grid& grid_clone) const {
    return make_shared<Knight>(grid_clone, colour, position);
}


vector<Move> Knight::getVisibleMoves() const {
    vector<Move> moves;

    vector<pair<int, int>> next_positions = scanKnight();

    // loop through all next positions and add the ones that are
    // empty or of the opposite colour
    for (const auto& next_position : next_positions) {

        // get the piece at the next position;
        shared_ptr<Piece> p = grid.getPiece(next_position);

        // add it if the piece is not ours
        if (!(p && p->getColour() == colour)) {
            Move move = {position, next_position};
            moves.emplace_back(move);
        }
    }

    return moves;
}
