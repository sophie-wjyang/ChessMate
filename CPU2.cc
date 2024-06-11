#include "CPU2.h"

CPU2::CPU2(const Grid& grid, Colour colour) : Player(grid, colour) {}

Move CPU2::getMove() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    Colour otherColour = colour == WHITE ? BLACK : WHITE;
    vector<Move> legalMoves = grid.getAllLegalMoves(colour);
    Move ret;

    // checks which of these moves are captures
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

    //check if chosen move is a pawn promotion
    if (ret.promote_to == 'Y') {
        //if it is, promote the pawn to a queen
        ret.promote_to = 'Q';
    }

    //print the move
    printMove(ret);

    return ret;

}
