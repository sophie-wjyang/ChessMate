#ifndef CS247_PROJECT_PIECE_H
#define CS247_PROJECT_PIECE_H

#include <utility>
#include "PieceId.h"
#include "Grid.h"
#include "Colour.h"
#include <vector>
#include <memory>
#include "Move.h"

using namespace std;

class Grid;

class Piece {
    protected:
        const Grid& grid;
        const PieceId id;
        Colour colour;
        pair<int, int> position;

        virtual vector<Move> getVisibleMoves() const = 0;

        vector<pair<int, int>> scanDirection(pair<int, int> direction) const;

        vector<pair<int, int>> scanKnight() const;

    public:
        Piece(PieceId id, const Grid& grid, Colour colour, const pair<int, int>& position);

        virtual shared_ptr<Piece> clone(const Grid& grid_clone) const = 0;

        void setPosition(const pair<int, int>& pos);

        const pair<int, int>& getPosition() const;

        vector<Move> getLegalMoves() const;

        bool isInDanger() const;

        Colour getColour() const;

        PieceId getId() const;

        bool isThreatenedBy(const Piece& p) const;

};


#endif //CS247_PROJECT_PIECE_H
