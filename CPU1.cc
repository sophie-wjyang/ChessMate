#include "CPU1.h"

CPU1::CPU1(const Grid& grid, Colour colour) : Player(grid, colour) {}

Move CPU1::getMove() {
    //get set of all legal moves
    vector<Move> legalMoves = grid.getAllLegalMoves(colour);
    //ranomly choose a move from this vector
    int randomIndex = rand() % legalMoves.size();

    Move ret = legalMoves[randomIndex];

    //check if chosen move is a pawn promotion
    if (ret.promote_to == 'Y') {
        // choose a random promotion, Q, R, B, N
        int promotionIndex = rand() % 4;
        char promotion;
        switch (promotionIndex) {
            case 0:
                promotion = 'Q';
                break;
            case 1:
                promotion = 'R';
                break;
            case 2:
                promotion = 'B';
                break;
            case 3:
                promotion = 'N';
                break;
        }
        ret.promote_to = promotion;
    }

    //print the move
    printMove(ret);

    return ret;
}
