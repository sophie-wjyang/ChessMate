#ifndef CS247_PROJECT_PLAYER_H
#define CS247_PROJECT_PLAYER_H

#include "Move.h"
#include "Grid.h"
#include <ctime>
#include <cstdlib>

using namespace std;

class Player {
    protected:
        const Grid& grid;
        const Colour colour;

    public:
        Player(const Grid& grid, Colour colour);

        virtual Move getMove() = 0;

        void printMove(Move move);
};


#endif //CS247_PROJECT_PLAYER_H
