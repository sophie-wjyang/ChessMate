#ifndef CS247_PROJECT_GRID_H
#define CS247_PROJECT_GRID_H

#include "Piece.h"
#include "BaseDisplay.h"
#include "Move.h"
#include "Colour.h"
#include <vector>
#include <memory>
#include <utility>

using namespace std;

class Piece;

class BaseDisplay;

class

Grid {
    private:
        vector<vector<shared_ptr<Piece>>> grid;
        shared_ptr<Piece> white_king;
        shared_ptr<Piece> black_king;
        vector<shared_ptr<BaseDisplay>> displays;
        Colour current_player;
        int turn;
        int moves_since_last_pawn_move_or_capture = 0;

    public:
        Grid();

        Grid clone() const;

        static bool withinBounds(pair<int, int> next_position);

        static bool withinBounds(int x, int y);

        static pair<int, int> positionFromNotation(const string& notation);

        void attachDisplay(shared_ptr<BaseDisplay> d);

        void detachDisplay(shared_ptr<BaseDisplay> d);

        void notifyDisplays() const;

        pair<int, int> resetKingPointers();

        bool isInCheck(Colour color) const;

        vector<Move> getAllLegalMoves(Colour colour) const;

        const shared_ptr<Piece>& getPiece(pair<int, int> position) const;

        const shared_ptr<Piece>& getPiece(int x, int y) const;

        shared_ptr<Piece>& getPiece(pair<int, int> position);

        shared_ptr<Piece>& getPiece(int x, int y);

        shared_ptr<Piece> makePiece(PieceId piece, Colour colour, pair<int, int> position, bool isSetup) const;

        void makeMove(Move move);

        void resetDefault();

        int getTurn() const;

        shared_ptr<Piece> getKing(Colour colour) const;

        Colour getCurrentPlayer() const;

        void setCurrentPlayer(Colour player);

        int& getMovesSinceLastPawnMoveOrCapture();

};

#endif //CS247_PROJECT_GRID_H
