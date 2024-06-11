#include "Piece.h"
#include "Utils.h"


Piece::Piece(const PieceId id, const Grid& grid, const Colour colour, const pair<int, int>& position)
        : grid{grid}, id{id}, colour{colour}, position{position} {}


vector<Move> Piece::getLegalMoves() const {
    // get all the visible moves
    vector<Move> visible_moves = getVisibleMoves();

    //for each move, create a copy of the board, make the move, and check if the king is in check
    vector<Move> legal_moves;
    for (auto& move: visible_moves) {
        // make a copy of the board
        Grid copy = grid.clone();
        // make the move on the copy
        copy.makeMove(move);

        // check if the king is in check
        if (!copy.isInCheck(colour)) {
            legal_moves.emplace_back(move);
        }
    }
    return legal_moves;
}


// returns a vector of all the possible next positions in the direction given
// stops when it hits a piece, but still includes it in the return vector
vector<pair<int, int>> Piece::scanDirection(pair<int, int> direction) const {
    vector<pair<int, int>> next_positions;

    pair<int, int> next_position = position;

    while (true) {
        // move one more step in the direction given
        next_position = make_pair(next_position.first + direction.first, next_position.second + direction.second);

        if (Grid::withinBounds(next_position)) {
            next_positions.emplace_back(next_position);

            // if the next position is not empty, include the piece but stop scanning
            if (grid.getPiece(next_position) != nullptr) {
                break;
            }
        }
        else {
            break;
        }
    }

    return next_positions;
}


// returns a vector of all the positions on the board that are a knight's move away
vector<pair<int, int>> Piece::scanKnight() const {
    vector<pair<int, int>> next_positions;
    pair<int, int> next_position;

    // when x is -2 or 2, y is -1 or 1
    // when x is -1 or 1, y is -2 or 2
    for (int x = -2; x <= 2; x++) {
        for (int y = -2; y <= 2; y++) {

            // not a knight's move if any of these are true
            if (x == 0 || y == 0 || abs(x) == abs(y)) continue;

            next_position = {position.first + x, position.second + y};

            // do not add this position if it is out of bounds
            if (!Grid::withinBounds(next_position)) continue;

            next_positions.emplace_back(next_position);
        }
    }

    return next_positions;
}


bool Piece::isInDanger() const {
    // check what colour the current piece is
    vector<pair<int, int>> pawns;
    Colour opposite_colour;

    if (colour == WHITE) {
        pawns = {{-1, 1},
                 {1,  1}};
        opposite_colour = BLACK;
    }
    else {
        pawns = {{-1, -1},
                 {1,  -1}};
        opposite_colour = WHITE;
    }

    vector<pair<int, int>> diagonals = {{1,  1},
                                        {1,  -1},
                                        {-1, 1},
                                        {-1, -1}};
    vector<pair<int, int>> straights = {{1,  0},
                                        {-1, 0},
                                        {0,  1},
                                        {0,  -1}};
    vector<pair<int, int>> enpassant_moves = {{-1, 0},
                                              {1,  0}};

    // space behind the pawn depending on white or black
    int behind = colour == WHITE ? -1 : 1;

    for (auto& direction: diagonals) {
        vector<pair<int, int>> spaces = scanDirection(direction);

        // nothing to check if nothing found in this direction
        if (spaces.empty()) continue;

        // get the last tile in the scan
        shared_ptr<Piece> p = grid.getPiece(spaces.back());

        // nothing to check if tile is empty
        if (!p) continue;

        // if the piece is the same colour, continue
        if (p->colour == colour) continue;

        //check it is the enemy colour, then check
        if (p->getId() == QUEEN || p->getId() == BISHOP) {
            // immediately return true if current piece is a king
            if (id == KING) return true;
            // call threatened by to check if the knight can capture the current piece
            if (this->isThreatenedBy(*p)) {
                return true;
            }
        }
        // check if the piece is a king
        else if (p->getId() == KING) {
            // check if the king is one tile away
            if (spaces.size() == 1) {
                // King cannot capture if the piece will be in check
                if (!this->isThreatenedBy(*p)){
                    return true;
                }
            }
        }
    }

    for (auto& direction: straights) {
        vector<pair<int, int>> spaces = scanDirection(direction);

        // nothing to check if nothing found in this direction
        if (spaces.empty()) continue;


        // get the last tile in the scan
        shared_ptr<Piece> p = grid.getPiece(spaces.back());

        // nothing to check if tile is empty
        if (!p) continue;

        // if the piece is the same colour, continue
        if (p->colour == colour) continue;

        //check it is the enemy colour, then check
        if (p->getId() == QUEEN || p->getId() == ROOK) {
            if (id == KING) return true;
            // call threatened by to check if the knight can capture the current piece
            if (this->isThreatenedBy(*p)) {
                return true;
            }
        }
        // check if the piece is a king
        else if (p->getId() == KING) {
            // check if the king is one tile away
            if (spaces.size() == 1) {
                // King cannot capture if the piece will be in check
                if (!this->isThreatenedBy(*p)){
                    return true;
                }
            }
        }
    }

    for (auto& direction: pawns) {
        vector<pair<int, int>> spaces = scanDirection(direction);

        // pawn must be exactly one tile away
        if (spaces.size() != 1) continue;

        // get the pawn's move tile
        shared_ptr<Piece> p = grid.getPiece(spaces.back());

        // nothing to check if tile is empty
        if (!p) continue;

        // if the piece is the same colour, continue
        if (p->colour == colour) continue;

        //check it is the enemy colour, then check if it is a pawn
        if (p->getId() == PAWN) {
            if (id == KING) return true;
            // call threatened by to check if the knight can capture the current piece
            if (this->isThreatenedBy(*p)) {
                return true;
            }
        }
    }

    // check for knights
    vector<pair<int, int>> knight_moves = scanKnight();
    for (auto& next_position: knight_moves) {
        // get the knight's move tile
        shared_ptr<Piece> p = grid.getPiece(next_position);

        // nothing to check if tile is empty
        if (!p) continue;

        // if the piece is the same colour, continue
        if (p->colour == colour) continue;

        // check it is the enemy colour, then check it is a knight
        if (p->getId() == KNIGHT) {
            if (id == KING) return true;
            // call threatened by to check if the knight can capture the current piece
            if (this->isThreatenedBy(*p)) {
                return true;
            }
        }
    }

    // check for enpassant
    // check if current piece is a pawn and that the space behind is empty
    if (id == PAWN && grid.withinBounds(position.first, position.second + behind) && !grid.getPiece(make_pair(position.first, position.second + behind))){
        for (auto& next_position: enpassant_moves) {
            // check if there exists a pawn piece in the next position
            if (grid.withinBounds(position.first + next_position.first, position.second + next_position.second)){
                shared_ptr<Piece> p = grid.getPiece(make_pair(position.first + next_position.first, position.second + next_position.second));
                if (p && p->getId() == PAWN && p->getColour() == opposite_colour){
                    // check if the space behind is in the legal moves of pawn
                    vector<Move> moves = p->getLegalMoves();
                    for (auto& move: moves){
                        if (move.to == make_pair(position.first, position.second + behind)){
                            return true;
                        }
                    }
                }
            }
        }
    }

    return false;
}

bool Piece::isThreatenedBy(const Piece& p) const{
    //checks the legal moves of the piece to see if it contains the current piece's position
    //TODO: This will not fully work for pawns when capturing via enpassant
    vector<Move> moves = p.getLegalMoves();

    for (auto& move: moves){
        if (move.to == position){
            return true;
        }
    }

    return false;
}


PieceId Piece::getId() const {
    return id;
}


Colour Piece::getColour() const {
    return colour;
}


void Piece::setPosition(const pair<int, int>& pos) {
    position = pos;
}


const pair<int, int>& Piece::getPosition() const {
    return position;
}
