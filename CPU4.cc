#include "CPU4.h"
#include <map>
#include <climits>
#include <iostream>

CPU4::CPU4(const Grid& grid, Colour colour): Player(grid, colour) {

}

Move CPU4::getMove() {
    //get set of all legal moves
    vector<Move> legalMoves = grid.getAllLegalMoves(colour);
    // for each move, perform the move and rate the grid
    int bestAdvantage = INT_MIN;
    bool allSame = true;
    bool isMax = colour == WHITE;

    //minimax
    pair<double, Move> res = minimax(grid, 0, 2, isMax, Move(), INT_MIN, INT_MAX);


    printMove(res.second);
    return res.second;
}


// evaluate
double CPU4::evaluate(const Grid& grid) const{
    double score = 0;

    // add advantage
    score += advantage(grid);

    // if black is in check, subtract 10
    if (grid.isInCheck(Colour::BLACK)) {
        score += 10;
        if (grid.getAllLegalMoves(Colour::BLACK).empty()) {
            score += 10000;
        }
    }

    // if enemy is in check, add 10
    if (grid.isInCheck(Colour::WHITE)) {
        score -= 10;
        if (grid.getAllLegalMoves(Colour::WHITE).empty()) {
            score -= 10000;
        }
    }

    return score;
}

// minimax algorithm
pair<double, Move> CPU4::minimax(const Grid& grid, int depth, int maxDepth, bool isMax, Move move, double alpha, double beta){
    // base case
    if (depth == maxDepth) {
        return make_pair(evaluate(grid), move);
    }
    Move bestMove;

    // if maximizing player (white)
    if(isMax){
        double maxEval = INT_MIN;
        // get all legal moves
        vector<Move> legalMoves = grid.getAllLegalMoves(Colour::WHITE);
        // for each move, perform the move and call minimax on the new grid
        for (Move m : legalMoves) {
            //check if  move is a pawn promotion
            if (m.promote_to == 'Y') {
                //if it is, promote the pawn to a queen
                m.promote_to = 'Q';
            }
            // make the move
            Grid copy = grid.clone();
            copy.makeMove(m);
            // call minimax on the new grid
            pair<double, Move> res = minimax(copy, depth + 1, maxDepth, false, m, alpha, beta);
            // update maxEval
            if (res.first > maxEval) {
                maxEval = res.first;
                bestMove = m;
            }
            // prune
            alpha = max(alpha, res.first);
            if (beta <= alpha){
                break;
            }
        }
        return make_pair(maxEval, bestMove);
    }
    // minimizing player (black)
    else {
        double minEval = INT_MAX;
        // get all legal moves
        vector<Move> legalMoves = grid.getAllLegalMoves(Colour::BLACK);
        // for each move, perform the move and call minimax on the new grid
        for (Move m : legalMoves) {
            //check if  move is a pawn promotion
            if (m.promote_to == 'Y') {
                //if it is, promote the pawn to a queen
                m.promote_to = 'Q';
            }
            // make the move
            Grid copy = grid.clone();
            copy.makeMove(m);
            // call minimax on the new grid
            pair<double, Move> res = minimax(copy, depth + 1, maxDepth, true, m, alpha, beta);
            // update minEval
            if (res.first < minEval) {
                minEval = res.first;
                bestMove = m;
            }
            // prune
            beta = min(beta, res.first);
            if (beta <= alpha){
                break;
            }
        }
        return make_pair(minEval, bestMove);
    }
}


double CPU4::advantage(const Grid& grid) const {
    std::map<PieceId, int> pieceValues = {
        {PAWN, 10},
        {KNIGHT, 30},
        {BISHOP, 30},
        {ROOK, 50},
        {QUEEN, 90},
        {KING, 9000}
    };

    std::map<PieceId, std::vector<std::vector<double>>> pieceTables = {
        {PAWN, {
                {0,   0,   0,   0,   0,   0,   0,   0},
                {5,   5,   5,   5,   5,   5,   5,   5},
                {1,   1,   2,   3,   3,   2,   1,   1},
                {.5, .5,   1, 2.5, 2.5,   1,  .5,  .5},
                {0,   0,   0,   2,   2,   0,   0,   0},
                {.5,-.5,  -1,   0,   0,  -1, -.5,  .5},
                {.5,  1,   1,  -2,  -2,   1,   1,  .5},
                {0,   0,   0,   0,   0,   0,   0,   0}}},
        {KNIGHT, {
                {-5, -4,  -3,  -3,  -3,  -3,  -4,  -5},
                {-4, -2,   0,   0,   0,   0,  -2,  -4},
                {-3,  0,   1, 1.5, 1.5,   1,   0,  -3},
                {-3, .5, 1.5,   2,   2, 1.5,  .5,  -3},
                {-3,  0, 1.5,   2,   2, 1.5,   0,  -3},
                {-3, .5,   1, 1.5, 1.5,   1,  .5,  -3},
                {-4, -2,   0,  .5,  .5,   0,  -2,  -4},
                {-5, -3,  -3,  -3,  -3,  -3,  -3,  -5}}},
        {BISHOP, {
                {-2, -1,  -1,  -1,  -1,  -1,  -1,  -2},
                {-1,  0,   0,   0,   0,   0,   0,  -1},
                {-1,  0,  .5,   1,   1,  .5,   0,  -1},
                {-1, .5,  .5,   1,   1,  .5,  .5,  -1},
                {-1,  0,   1,   1,   1,   1,   0,  -1},
                {-1,  1,   1,   1,   1,   1,   1,  -1},
                {-1, .5,   0,   0,   0,   0,  .5,  -1},
                {-2, -1,  -1,  -1,  -1,  -1,  -1,  -2}}},
        {ROOK, {
                {0,   0,   0,   0,   0,   0,   0,   0},
                {.5,  1,   1,   1,   1,   1,   1,  .5},
                {-.5,  0,   0,   0,   0,   0,   0, -.5},
                {-.5,  0,   0,   0,   0,   0,   0, -.5},
                {-.5,  0,   0,   0,   0,   0,   0, -.5},
                {-.5,  0,   0,   0,   0,   0,   0, -.5},
                {-.5,  0,   0,   0,   0,   0,   0, -.5},
                {0,   0,  .5,  .5,  .5,  .5,   0,   0}}},
        {QUEEN, {
                {-2, -1,  -1, -.5, -.5,  -1,  -1,  -2},
                {-1,  0,   0,   0,   0,   0,   0,  -1},
                {-1,  0,  .5,  .5,  .5,  .5,   0,  -1},
                {-.5,  0,  .5,  .5,  .5,  .5,   0, -.5},
                {0,   0,  .5,  .5,  .5,  .5,   0, -.5},
                {-1,  .5,  .5,  .5,  .5,  .5,   0,  -1},
                {-1,  0,  .5,   0,   0,   0,   0,  -1},
                {-2, -1,  -1, -.5, -.5,  -1,  -1,  -2}}},
        {KING, {
                {-3, -4,  -4,  -5,  -5,  -4,  -4,  -3},
                {-3, -4,  -4,  -5,  -5,  -4,  -4,  -3},
                {-3, -4,  -4,  -5,  -5,  -4,  -4,  -3},
                {-3, -4,  -4,  -5,  -5,  -4,  -4,  -3},
                {-2, -3,  -3,  -4,  -4,  -3,  -3,  -2},
                {-1, -2,  -2,  -2,  -2,  -2,  -2,  -1},
                {2,   2,   0,   0,   0,   0,   2,   2},
                {2,  3.5,  1,   0,   0,   1, 3.5,   2}}}
    };

    double advantage = 0;
    for (int x = 0; x < 8; x++){
        for (int y = 0; y < 8; y++){
            shared_ptr<Piece> p = grid.getPiece(x, y);
            if (p){
                // Determine the color and add/subtract the piece value from the advantage
                if (p->getColour() == Colour::WHITE){
                    // material advantage
                    advantage += pieceValues[p->getId()];
                    // positional advantage
                    advantage += pieceTables[p->getId()][x][y];

                } else {
                    // material advantage
                    advantage -= pieceValues[p->getId()];
                    // positional advantage
                    advantage -= pieceTables[p->getId()][x][7-y];
                }
            }
        }
    }
    return advantage;
}
