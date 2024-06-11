#ifndef CS247_PROJECT_QUEEN_H
#define CS247_PROJECT_QUEEN_H

#include "Piece.h"

using namespace std;

class Queen : public Piece {
    protected:
        vector<Move> getVisibleMoves() const override;

    public:
        Queen(const Grid& grid, Colour colour, const pair<int, int>& position);

        shared_ptr<Piece> clone(const Grid& grid_clone) const override;
};


#endif //CS247_PROJECT_QUEEN_H
