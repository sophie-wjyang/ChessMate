//
// Created by fredy on 2023-07-21.
//

#ifndef CS247_PROJECT_MOVE_H
#define CS247_PROJECT_MOVE_H

#include <utility>
#include <iostream>

using namespace std;

struct Move {
    pair<int, int> from;
    pair<int, int> to;
    char promote_to = 'X';

    static const pair<int, int> invalid;

    bool operator==(const Move& other) const;

    bool operator!=(const Move& other) const;

    friend ostream& operator<<(ostream& os, const Move& move);
};

#endif //CS247_PROJECT_MOVE_H
