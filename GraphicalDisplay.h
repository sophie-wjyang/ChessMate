#ifndef CS247_PROJECT_GRAPHICALDISPLAY_H
#define CS247_PROJECT_GRAPHICALDISPLAY_H

#include "BaseDisplay.h"
#include <utility>
#include "window.h"

using namespace std;

class GraphicalDisplay : public BaseDisplay {
    private:
        Xwindow window;

        Grid prev_state;

        void drawTile(pair<int, int> position);

    public:
        explicit GraphicalDisplay(const Grid& grid);

        void notify() override;
};


#endif //CS247_PROJECT_GRAPHICALDISPLAY_H
