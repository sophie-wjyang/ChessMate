#include "King.h"
#include "Rook.h"

King::King(const Grid& grid, Colour colour, const pair<int, int>& position, const bool has_moved = false)
        : Piece(KING, grid, colour, position), has_moved{has_moved} {}


shared_ptr<Piece> King::clone(const Grid& grid_clone) const {
    return make_shared<King>(grid_clone, colour, position, has_moved);
}

vector<Move> King::getVisibleMoves() const {
    vector<Move> moves;
    pair<int, int> next_position;

    // moves one square in any direction
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            if (x == 0 && y == 0) {
                continue;
            }

            next_position = {position.first + x, position.second + y};

            // not visible if out of bounds
            if (!Grid::withinBounds(next_position)) continue;

            // get the tile at the next position
            shared_ptr<Piece> p = grid.getPiece(next_position);

            // tile is not visible if it belongs to the same person
            if (p && p->getColour() == colour) continue;

            // tile is not visible if it is next to the other person's king
            Colour other_player = colour == WHITE ? BLACK : WHITE;
            pair<int, int> other_king_position = grid.getKing(other_player)->getPosition();
            int x_diff = other_king_position.first - next_position.first;
            int y_diff = other_king_position.second - next_position.second;

            if (abs(x_diff) <= 1 && abs(y_diff) <= 1) continue;

            // if all other checks pass, then this move is visible
            Move move = {position, next_position};
            moves.emplace_back(move);
        }
    }

    // castling
    // requirement 1: king hasn't moved and rook hasn't moved
    // requirement 2: no pieces between king and rook
    // requirement 3: king is not in check
    // requirement 4: king does not move through check
    if (!has_moved) {
        // right side castling (king-side castling)
        if (canRightSideCastle()){
            Move move = {position, {position.first + 2, position.second}};
            moves.emplace_back(move);
        }

        // left side castling (queen-side castling)
        if (canLeftSideCastle()){
            Move move = {position, {position.first - 2, position.second}};
            moves.emplace_back(move);
        }

    }

    return moves;
}

bool King::canRightSideCastle() const{
    shared_ptr<Piece> rook = grid.getPiece(position.first + 3, position.second);

    // check that the piece is a rook
    if(!(rook && rook->getId() == ROOK)){
        return false;
    }

    // check that the rook hasn't moved
    if(dynamic_pointer_cast<Rook>(rook)->getHasMoved()){
        return false;
    }

    // check that there are no pieces between king and rook
    if(grid.getPiece(position.first + 1, position.second) != nullptr ||
        grid.getPiece(position.first + 2, position.second) != nullptr
    ){
        return false;
    }

    // check that king isn't in check
    if(grid.getPiece(position.first, position.second)->isInDanger()){
        return false;
    }

    // check that the king doesn't move through check
    Grid grid_copy = grid.clone();

    grid_copy.makeMove({{position.first, position.second}, {position.first + 1, position.second}});

    if(grid_copy.getPiece(position.first + 1, position.second)->isInDanger()){
        return false;
    }

    grid_copy.makeMove({{position.first + 1, position.second}, {position.first + 2, position.second}});

    if(grid_copy.getPiece(position.first + 2, position.second)->isInDanger()){
        return false;
    }

    return true;
}

bool King::canLeftSideCastle() const{
    shared_ptr<Piece> rook = grid.getPiece(position.first - 4, position.second);

    // check that the piece is a rook
    if(!(rook && rook->getId() == ROOK)){
        return false;
    }

    // check that the rook hasn't moved
    if(dynamic_pointer_cast<Rook>(rook)->getHasMoved()){
        return false;
    }

    // check that there are no pieces between king and rook
    if(grid.getPiece(position.first - 1, position.second) != nullptr ||
        grid.getPiece(position.first - 2, position.second) != nullptr
    ){
        return false;
    }

    // check that king isn't in check
    if(grid.getPiece(position.first, position.second)->isInDanger()){
        return false;
    }

    // check that the king doesn't move through check
    Grid grid_copy = grid.clone();

    grid_copy.makeMove({{position.first, position.second}, {position.first - 1, position.second}});

    if(grid_copy.getPiece(position.first - 1, position.second)->isInDanger()){
        return false;
    }

    grid_copy.makeMove({{position.first - 1, position.second}, {position.first - 2, position.second}});

    if(grid_copy.getPiece(position.first - 2, position.second)->isInDanger()){
        return false;
    }

    return true;
}


void King::setHasMoved(bool val) {
    has_moved = val;
}
