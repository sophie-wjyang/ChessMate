#ifndef CS247_PROJECT_KING_H
#define CS247_PROJECT_KING_H

#include "Piece.h"

using namespace std;

class King : public Piece {
    private:
        bool has_moved;

    protected:
        vector<Move> getVisibleMoves() const override;

    public:
        King(const Grid& grid, Colour colour, const pair<int, int>& position, bool has_moved);

        shared_ptr<Piece> clone(const Grid& grid_clone) const override;

        bool canRightSideCastle() const;

        bool canLeftSideCastle() const;

        void setHasMoved(bool val);
};


#endif //CS247_PROJECT_KING_H
