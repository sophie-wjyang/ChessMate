#ifndef CS247_PROJECT_CPU1_H
#define CS247_PROJECT_CPU1_H

#include "Player.h"

using namespace std;

class CPU1 : public Player {
    public:
        CPU1(const Grid& grid, Colour colour);

        Move getMove() override;
};


#endif //CS247_PROJECT_CPU1_H
