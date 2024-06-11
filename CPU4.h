#ifndef CS247_PROJECT_CPU4_H
#define CS247_PROJECT_CPU4_H

#include "Player.h"

using namespace std;

class CPU4 : public Player {
    public:
        CPU4(const Grid& grid, Colour colour);

        Move getMove() override;

        double advantage(const Grid& grid) const;

        double evaluate(const Grid& grid) const;

        pair<double, Move> minimax(const Grid& grid, int depth, int maxDepth, bool isMax, Move move, double alpha, double beta);
};


#endif //CS247_PROJECT_CPU4_H
