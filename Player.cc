#include "Player.h"

Player::Player(const Grid& grid, Colour colour) : grid{grid}, colour{colour} {}

// implement print move
void Player::printMove(Move move) {
    // find the piece at the from position shared pointer
    shared_ptr<Piece> p = grid.getPiece(move.from);
    // find the string colour of the piece without colortoString
    string colourString;
    if (p->getColour() == Colour::BLACK) {
        colourString = "Black";
    } else {
        colourString = "White";
    }
    // find the string id of the piece without idToString
    string idString;
    if (p->getId() == PieceId::PAWN) {
        idString = "Pawn";
    } else if (p->getId() == PieceId::ROOK) {
        idString = "Rook";
    } else if (p->getId() == PieceId::KNIGHT) {
        idString = "Knight";
    } else if (p->getId() == PieceId::BISHOP) {
        idString = "Bishop";
    } else if (p->getId() == PieceId::QUEEN) {
        idString = "Queen";
    } else if (p->getId() == PieceId::KING) {
        idString = "King";
    }

    // if the move is a pawn promotion
    if (move.promote_to != 'X') {
        cout << colourString << " moves " << idString << " from " << move << endl;
        string promoteString;
        if (move.promote_to == 'Q'){
            promoteString = "Queen";
        } else if (move.promote_to == 'R'){
            promoteString = "Rook";
        } else if (move.promote_to == 'B'){
            promoteString = "Bishop";
        } else if (move.promote_to == 'N'){
            promoteString = "Knight";
        }
        cout << colourString << " promotes pawn to " << promoteString << " on " << move << endl;
    } else {
        // print the move
        cout << colourString << " moves " << idString << " from " << move << endl;
    }
}
