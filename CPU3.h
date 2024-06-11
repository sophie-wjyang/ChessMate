#ifndef CS247_PROJECT_CPU3_H
#define CS247_PROJECT_CPU3_H

#include "Player.h"

using namespace std;

class CPU3 : public Player {
    public:
        CPU3(const Grid& grid, Colour colour);

        Move getMove() override;

};


#endif //CS247_PROJECT_CPU3_H
