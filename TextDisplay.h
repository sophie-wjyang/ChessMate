#ifndef CS247_PROJECT_TEXTDISPLAY_H
#define CS247_PROJECT_TEXTDISPLAY_H

#include "BaseDisplay.h"

using namespace std;

class TextDisplay : public BaseDisplay {
    public:
        explicit TextDisplay(const Grid& grid);

        void notify() override;
};


#endif //CS247_PROJECT_TEXTDISPLAY_H
