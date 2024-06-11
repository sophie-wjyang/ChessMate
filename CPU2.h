#ifndef CS247_PROJECT_CPU2_H
#define CS247_PROJECT_CPU2_H

#include "Player.h"

using namespace std;

class CPU2 : public Player {
    public:
        CPU2(const Grid& grid, Colour colour);

        Move getMove() override;
};

#endif //CS247_PROJECT_CPU2_H
