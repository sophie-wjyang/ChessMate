#include "CPU3.h"

CPU3::CPU3(const Grid& grid, Colour colour) : Player(grid, colour) {

}

Move CPU3::getMove() {
    Colour otherColour = colour == WHITE ? BLACK : WHITE;
    Move ret;
    //get set of all legal moves
    vector<Move> legalMoves = grid.getAllLegalMoves(colour);

    // all pieces that are in danger
    vector<shared_ptr<Piece>> piecesInDanger;

    // for each piece of the current colour
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            shared_ptr<Piece> p = grid.getPiece(x, y);
            // check that the current piece exists and it is the same colour
            if (!p) continue;
            if (p->getColour() != colour) continue;

            // if (!(grid.getPiece(x, y) && grid.getPiece(x, y)->getColour() == colour)) continue;

            // if the piece is in danger, add it to the vector
            if (p && p->isInDanger()) {
                piecesInDanger.push_back(grid.getPiece(x, y));
            }
        }
    }

    // for all Pieces in piecesInDanger, check which legal moves avoid capture
    vector<Move> safeMoves;
    for (shared_ptr<Piece> piece : piecesInDanger) {
        // get the legal moves
        for (Move move : piece->getLegalMoves()) {
            // check if the move results in inDanger();
            Grid copy = grid.clone();
            copy.makeMove(move);
            // if the move does not result in inDanger(), add it to the vector
            shared_ptr<Piece> p = copy.getPiece(move.to);
            if (!p->isInDanger()) {
                safeMoves.push_back(move);
            }
        }
    }

    // randomly choose a move from this vector if not empty
    if (!safeMoves.empty()) {
        int randomIndex = rand() % safeMoves.size();
        ret = safeMoves[randomIndex];
    } else {
        //check if there are any captures
        vector<Move> captureMoves;
        for (Move move : legalMoves) {
            // check if the move to has a piece of the other colour
            if (grid.getPiece(move.to) && grid.getPiece(move.to)->getColour() != colour) {
                captureMoves.push_back(move);
                continue;
            }
            // check if the move results in a check
            Grid copy = grid.clone();
            copy.makeMove(move);
            if (copy.isInCheck(otherColour)) {
                // add the move to the capture moves
                captureMoves.push_back(move);
            }

            //TODO: This does not implement enpassants
        }
            // if there are no captures, return a random move
        if (captureMoves.empty()) {
            int randomIndex = rand() % legalMoves.size();
            ret = legalMoves[randomIndex];
        } else {
            // choose a random capture
            int randomIndex = rand() % captureMoves.size();
            ret = captureMoves[randomIndex];
        }
    }

    //check if chosen move is a pawn promotion
    if (ret.promote_to == 'Y') {
        //if it is, promote the pawn to a queen
        ret.promote_to = 'Q';
    }

    //print the move
    printMove(ret);

    return ret;
}
