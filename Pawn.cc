#include "Pawn.h"


Pawn::Pawn(const Grid& grid, Colour colour, const pair<int, int>& position, const int double_move_turn = -1)
        : Piece(PAWN, grid, colour, position), double_move_turn{double_move_turn} {}


shared_ptr<Piece> Pawn::clone(const Grid& grid_clone) const {
    return make_shared<Pawn>(grid_clone, colour, position, double_move_turn);
}


vector<Move> Pawn::getVisibleMoves() const {
    vector<Move> moves;
    // white pawns move up (y + 1), black pawns move down (y - 1)
    int forward_direction = (colour == WHITE) ? 1 : -1;

    // standard move: one square forward
    pair<int, int> next_position = make_pair(position.first, position.second + forward_direction);

    if (Grid::withinBounds(next_position) && grid.getPiece(next_position) == nullptr) {
        Move move = {position, next_position};
        // if move is a pawn promotion, mark promote_to as 'Y' to indicate pawn promotion
        if (next_position.second == 0 || next_position.second == 7) {
            move.promote_to = 'Y';
        }
        moves.emplace_back(move);

        // first move: two squares forward
        if ((colour == WHITE && position.second == 1) || (colour == BLACK && position.second == 6)) {
            next_position = make_pair(position.first, position.second + 2 * forward_direction);

            if (Grid::withinBounds(next_position) && grid.getPiece(next_position) == nullptr) {
                move = {position, next_position};
                moves.emplace_back(move);
            }
        }
    }

    // capturing move: diagonal capture
    // one square forward + one square to the left or right
    for (int x_offset = -1; x_offset <= 1; x_offset += 2) {
        next_position = make_pair(position.first + x_offset, position.second + forward_direction);

        // the next position must currently have a piece of the opposite colour
        if (Grid::withinBounds(next_position) && grid.getPiece(next_position) != nullptr &&
            grid.getPiece(next_position)->getColour() != colour) {
            Move move = {position, next_position};
            // if move is a pawn promotion, mark promote_to as 'Y' to indicate pawn promotion
            if (next_position.second == 0 || next_position.second == 7) {
                move.promote_to = 'Y';
            }
            moves.emplace_back(move);
        }
    }

    // en passant
    // requirement 1: the captured pawn must have moved two squares in one move, landing right next to the capturing pawn
    // requirement 2: the en passant capture must be performed on the turn immediately after the pawn being captured moves
    for (int x_offset = -1; x_offset <= 1; x_offset += 2) {
        // check both left and right of the pawn
        pair<int, int> adjacent_position = make_pair(position.first + x_offset, position.second);

        if (Grid::withinBounds(adjacent_position)) {
            shared_ptr<Piece> adjacent_piece = grid.getPiece(adjacent_position);

            // check if the adjacent piece 1) exists, 2) is a pawn of the opposite color, 3) moved two squares on its previous move
            if (adjacent_piece != nullptr &&
                adjacent_piece->getId() == PAWN &&
                adjacent_piece->getColour() != colour &&
                dynamic_pointer_cast<Pawn>(adjacent_piece)->double_move_turn == grid.getTurn() - 1)
            {
                next_position = make_pair(adjacent_position.first, adjacent_position.second + forward_direction);

                // check that the position you want to move to is within bounds and empty
                if (Grid::withinBounds(next_position) && grid.getPiece(next_position) == nullptr) {
                    Move move = {position, next_position};
                    moves.emplace_back(move);
                }
            }
        }
    }


    return moves;
}

void Pawn::setDoubleMoveTurn(int turn){
    double_move_turn = turn;
}
