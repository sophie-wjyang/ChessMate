#ifndef CS247_PROJECT_BASEDISPLAY_H
#define CS247_PROJECT_BASEDISPLAY_H

#include "Grid.h"

using namespace std;

class Grid;

class BaseDisplay {
    protected:
        const Grid& grid;
    public:
        explicit BaseDisplay(const Grid& grid);

        virtual void notify() = 0;
};


#endif //CS247_PROJECT_BASEDISPLAY_H
