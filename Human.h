#ifndef CS247_PROJECT_HUMAN_H
#define CS247_PROJECT_HUMAN_H

#include "Player.h"

using namespace std;

class Human : public Player {
    private:
        static Move interpretMoveCommand(const vector<string>& command);

    public:
        explicit Human(const Grid& grid, Colour colour);

        Move getMove() override;
};


#endif //CS247_PROJECT_HUMAN_H
