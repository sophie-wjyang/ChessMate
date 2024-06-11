#ifndef CS247_PROJECT_ROOK_H
#define CS247_PROJECT_ROOK_H

#include "Piece.h"

using namespace std;

class Rook : public Piece {
    private:
        bool has_moved;

    protected:
        vector<Move> getVisibleMoves() const override;

    public:
        Rook(const Grid& grid, Colour colour, const pair<int, int>& position, bool has_moved);

        shared_ptr<Piece> clone(const Grid& grid_clone) const override;

        bool getHasMoved() const;

        void setHasMoved(bool val);

};


#endif //CS247_PROJECT_ROOK_H
