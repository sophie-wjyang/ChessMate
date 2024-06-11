#include "Grid.h"
#include "Colour.h"
#include <algorithm>
#include <stdexcept>
#include "Pawn.h"
#include "Knight.h"
#include "Queen.h"
#include "King.h"
#include "Rook.h"
#include "Bishop.h"
#include <iostream>
#include <utility>
#include <memory>


Grid::Grid() : grid{8, vector<shared_ptr<Piece>>(8, nullptr)}, turn{1}, current_player{WHITE} {}


Grid Grid::clone() const {
    Grid grid_clone;

    // copy turn count
    grid_clone.turn = turn;

    // clone the pieces in the grid
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            const auto& piece = getPiece(i, j);

            // do not clone if no piece in this tile
            if (!piece) continue;

            // clone the piece
            shared_ptr<Piece> piece_clone = piece->clone(grid_clone);

            // put it into the cloned grid
            grid_clone.grid.at(i).at(j) = piece_clone;

            // if it is the white king, track that in the grid clone
            if (white_king == piece) {
                grid_clone.white_king = piece_clone;
            }

            // if it is the black king, track that in the grid clone
            if (black_king == piece) {
                grid_clone.black_king = piece_clone;
            }
        }
    }

    return grid_clone;
}


bool Grid::withinBounds(int x, int y) {
    return x >= 0 && x < 8 && y >= 0 && y < 8;
}


bool Grid::withinBounds(pair<int, int> next_position) {
    int x = next_position.first;
    int y = next_position.second;

    return withinBounds(x, y);
}


pair<int, int> Grid::positionFromNotation(const string& notation) {

    if (notation.size() == 2) {
        int x = notation.at(0) - 'a';
        int y = notation.at(1) - '1';

        if (withinBounds(x, y)) {
            return {x, y};
        }
    }

    cout << "Invalid tile: " << notation << endl;
    return {-1, -1};
}


void Grid::attachDisplay(shared_ptr<BaseDisplay> d) {
    displays.push_back(d);
}


void Grid::detachDisplay(shared_ptr<BaseDisplay> d) {
    displays.erase(remove(displays.begin(), displays.end(), d), displays.end());
}


void Grid::notifyDisplays() const {
    for (const auto& d: displays) {
        d->notify();
    }
}


vector<Move> Grid::getAllLegalMoves(Colour colour) const {
    vector<Move> moves;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (grid[i][j] != nullptr && grid[i][j]->getColour() == colour) {
                // get the legal moves for each piece, and add it to the large vector of all legal moves a player can make
                vector<Move> piece_moves = grid[i][j]->getLegalMoves();
                moves.insert(moves.end(), piece_moves.begin(), piece_moves.end());
            }
        }
    }

    return moves;
}


bool Grid::isInCheck(Colour color) const {
    if (color == WHITE) {
        return white_king->isInDanger();
    }
    else {
        return black_king->isInDanger();
    }
}


const shared_ptr<Piece>& Grid::getPiece(const int x, const int y) const {
    return grid.at(x).at(y);
}


const shared_ptr<Piece>& Grid::getPiece(pair<int, int> position) const {
    return getPiece(position.first, position.second);
}


shared_ptr<Piece>& Grid::getPiece(const int x, const int y) {
    return grid.at(x).at(y);
}

shared_ptr<Piece>& Grid::getPiece(pair<int, int> position) {
    return getPiece(position.first, position.second);
}

void Grid::makeMove(Move move) {
    auto& from_piece = getPiece(move.from);
    auto& to_piece = getPiece(move.to);

    // set moves_since_last_pawn_move flag
    if(from_piece->getId() == PAWN || to_piece != nullptr){
        getMovesSinceLastPawnMoveOrCapture() = 0;
    }
    else{
        getMovesSinceLastPawnMoveOrCapture()++;
    }


    // check if it's an en passant: check if the move is 1) a pawn, 2) moving diagonally, 3) into an empty space
    if((from_piece->getId() == PAWN) &&
        (abs(move.to.first - move.from.first) == 1 && abs(move.to.second - move.from.second) == 1) &&
        (to_piece == nullptr))
    {
        // if so, remove the pawn that is being captured
        getPiece(move.to.first, move.from.second) = nullptr;
    }

    // check if it's castling: check if the move is 1) a king, 2) moving 2 spaces horizontally
    else if((from_piece->getId() == KING) &&
        (abs(move.to.first - move.from.first) == 2))
    {
        // white castling
        if(from_piece->getColour() == WHITE){
            if(move.to.first == 2){
                // move left rook from (0, 0) to (3, 0)
                getPiece(0, 0)->setPosition({3, 0});
                getPiece(3, 0) = getPiece(0, 0);
                getPiece(0, 0) = nullptr;
            }
            else if(move.to.first == 6){
                // move left rook from (7, 0) to (5, 0)
                getPiece(7, 0)->setPosition({5, 0});
                getPiece(5, 0) = getPiece(7, 0);
                getPiece(7, 0) = nullptr;
            }
        }
        // black castling
        else{
            if(move.to.first == 2){
                // move left rook from (0, 7) to (3, 7)
                getPiece(0, 7)->setPosition({3, 7});
                getPiece(3, 7) = getPiece(0, 7);
                getPiece(0, 7) = nullptr;
            }
            else if(move.to.first == 6){
                // move left rook from (7, 7) to (5, 7)
                getPiece(7, 7)->setPosition({5, 7});
                getPiece(5, 7) = getPiece(7, 7);
                getPiece(7, 7) = nullptr;
            }
        }
    }
    // set double_move_turn, if a pawn moves forward by two squares
    else if(from_piece->getId() == PAWN &&
        abs(move.to.second - move.from.second) == 2)
    {
        dynamic_pointer_cast<Pawn>(from_piece)->setDoubleMoveTurn(turn);
    }


    // set hasMoved for Kings/Rooks
    if (from_piece->getId() == KING) {
        dynamic_pointer_cast<King>(from_piece)->setHasMoved(true);
    }
    else if (from_piece->getId() == ROOK) {
        dynamic_pointer_cast<Rook>(from_piece)->setHasMoved(true);
    }


    // if the move sequence is a pawn promotion but the player doesn't specify what to promote to
    // if the piece at the position move.from is a pawn

    if(getPiece(move.from)->getId() == PAWN &&
        ((move.from.second == 1 && move.to.second == 0) || (move.from.second == 6 && move.to.second == 7)) &&
        move.promote_to == 'X')
    {
        // default promotion: pawn to queen
        shared_ptr<Piece> new_queen = makePiece(QUEEN, from_piece->getColour(), move.to, false);
        // set the piece at move.to to be the new queen
        from_piece = new_queen;
    }


    // check if it's a pawn promotion
    if(move.promote_to != 'X'){
        // check if Q
        if(move.promote_to == 'Q'){
            // create a new queen
            shared_ptr<Piece> new_queen = makePiece(QUEEN, from_piece->getColour(), move.to, false);
            // set the piece at move.to to be the new queen
            from_piece = new_queen;
        }
        // check if R
        else if(move.promote_to == 'R'){
            // create a new rook
            shared_ptr<Piece> new_rook = makePiece(ROOK, from_piece->getColour(), move.to, false);
            // set the piece at move.to to be the new rook
            from_piece = new_rook;
        }
        // check if B
        else if(move.promote_to == 'B'){
            // create a new bishop
            shared_ptr<Piece> new_bishop = makePiece(BISHOP, from_piece->getColour(), move.to, false);
            // set the piece at move.to to be the new bishop
            from_piece = new_bishop;
        }
        // check if N
        else if(move.promote_to == 'N'){
            // create a new knight
            shared_ptr<Piece> new_knight = makePiece(KNIGHT, from_piece->getColour(), move.to, false);
            // set the piece at move.to to be the new knight
            from_piece = new_knight;
        }
    }

    // standard moves
    from_piece->setPosition(move.to);
    to_piece = from_piece;
    from_piece = nullptr;

    turn++;
    current_player = (current_player == WHITE ? BLACK : WHITE);
}

shared_ptr<Piece> Grid::makePiece(PieceId piece, Colour colour, pair<int, int> position, bool isSetup) const {
    switch (piece) {
        case ROOK:
            return make_shared<Rook>(*this, colour, position, isSetup);
        case KING:
            return make_shared<King>(*this, colour, position, isSetup);
        case PAWN:
            return make_shared<Pawn>(*this, colour, position, -1);
        case QUEEN:
            return make_shared<Queen>(*this, colour, position);
        case BISHOP:
            return make_shared<Bishop>(*this, colour, position);
        case KNIGHT:
            return make_shared<Knight>(*this, colour, position);
        default:
            throw invalid_argument("Invalid PieceId enum Grid::makePiece");
    }
}

// resets all pieces in the grid to default board configuration
void Grid::resetDefault() {
    // back rank piece configuration
    vector<PieceId> back_rank_pieces{ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK};

    // sets all pieces in the grid
    for (int i = 0; i < back_rank_pieces.size(); ++i) {
        PieceId piece_id = back_rank_pieces.at(i);

        // set black and white back rank
        getPiece(i, 0) = makePiece(piece_id, WHITE, {i, 0}, false);
        getPiece(i, 7) = makePiece(piece_id, BLACK, {i, 7}, false);

        // set black and white pawns
        getPiece(i, 1) = makePiece(PAWN, WHITE, {i, 1}, false);
        getPiece(i, 6) = makePiece(PAWN, BLACK, {i, 6}, false);

        // set center tiles to be empty
        for (int j = 2; j < 6; ++j) {
            getPiece(i, j) = nullptr;
        }
    }

    // sets references to kings
    white_king = getPiece(4, 0);
    black_king = getPiece(4, 7);

    turn = 1;
    moves_since_last_pawn_move_or_capture = 0;
    current_player = WHITE;
}


pair<int, int> Grid::resetKingPointers() {
    pair<int, int> king_counts = {0, 0};
    white_king = nullptr;
    black_king = nullptr;

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            const auto& piece = getPiece(i, j);

            if (!(piece && piece->getId() == KING)) continue;

            if (piece->getColour() == WHITE) {
                white_king = piece;
                king_counts.first++;
            }
            else {
                black_king = piece;
                king_counts.second++;
            }
        }
    }

    return king_counts;
}

int Grid::getTurn() const{
    return turn;
}

shared_ptr<Piece> Grid::getKing(Colour colour) const {
    if (colour == WHITE) {
        return white_king;
    }
    else {
        return black_king;
    }
}


Colour Grid::getCurrentPlayer() const {
    return current_player;
}


void Grid::setCurrentPlayer(Colour player) {
    current_player = player;
}

int& Grid::getMovesSinceLastPawnMoveOrCapture(){
    return moves_since_last_pawn_move_or_capture;
}
