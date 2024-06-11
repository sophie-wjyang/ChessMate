#ifndef CS247_PROJECT_BISHOP_H
#define CS247_PROJECT_BISHOP_H

#include "Piece.h"

using namespace std;

class Bishop : public Piece {
    protected:
        vector<Move> getVisibleMoves() const override;

    public:
        Bishop(const Grid& grid, Colour colour, const pair<int, int>& position);

        shared_ptr<Piece> clone(const Grid& grid_clone) const override;
};


#endif //CS247_PROJECT_BISHOP_H
