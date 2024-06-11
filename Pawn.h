#ifndef CS247_PROJECT_PAWN_H
#define CS247_PROJECT_PAWN_H

#include "Piece.h"

using namespace std;

class Pawn : public Piece {
    private:
        int double_move_turn;

    protected:
        vector<Move> getVisibleMoves() const override;

    public:
        Pawn(const Grid& grid, Colour colour, const pair<int, int>& position, int double_move_turn);

        shared_ptr<Piece> clone(const Grid& grid_clone) const override;

        void setDoubleMoveTurn(int turn);
};


#endif //CS247_PROJECT_PAWN_H
