#include "Move.h"

const pair<int, int> Move::invalid = {-1, -1};


bool Move::operator==(const Move& other) const {
    return from == other.from && to == other.to && promote_to == other.promote_to;
}


bool Move::operator!=(const Move& other) const {
    return !(*this == other);
}

ostream& operator<<(ostream& os, const Move& move) {
    // change this to be more like the chess notation
    // y values need to be 1 indexed
    // x values need to be a-h
    char x1 = move.from.first + 'a';
    char x2 = move.to.first + 'a';
    int y1 = move.from.second + 1;
    int y2 = move.to.second + 1;

    os << x1 << y1 << " -> " << x2 << y2;
    return os;
}
