#include "Human.h"
#include <iostream>
#include <algorithm>
#include "Utils.h"

Human::Human(const Grid& grid, Colour colour) : Player(grid, colour) {}

Move Human::getMove() {
    string line;
    vector<string> command;

    while (true) {
        // cli prompt
        cout << ">> ";

        // exit if read fails
        if (!getline(cin, line)) return {Move::invalid, Move::invalid};

        // parse command into parts
        command = splitString(line);

        // re-prompt if command was empty
        if (command.empty()) continue;

        // case for resignation
        if (command.front() == "resign") {
            // return an invalid move
            return {Move::invalid, Move::invalid};
        }
        // case for moving a piece
        else if (command.front() == "move") {
            // re-prompt if not enough arguments
            if (command.size() < 3) {
                cout << "Usage: move <tile1> <tile2> [promotion]" << endl;
                continue;
            }

            // interpret command into move object
            Move move = interpretMoveCommand(command);

            // re-prompt if move is invalid
            if (move.from == Move::invalid) continue;

            // get the piece we are moving
            const shared_ptr<Piece>& from_piece = grid.getPiece(move.from);

            // re-prompt if `from` position does not contain a piece
            if (!from_piece) {
                cout << "Invalid move: " <<  command.at(1) << " is empty" << endl;
                continue;
            }

            // re-prompt if `from` piece is not of the same colour
            if (colour != from_piece->getColour()) {
                cout << "Invalid move: " << command.at(1) << " belongs to the other player" << endl;
                continue;
            }

            // if pawn promotion, check that the fourth argument is a valid char
            if(command.size() >= 4){
                if (from_piece->getId() != PAWN) {
                    cout << "Invalid move: Cannot promote a non pawn piece" << endl;
                    continue;
                }

                if((command.at(3).size() != 1) ||
                   (command.at(3).at(0) != 'q' && command.at(3).at(0) != 'Q' &&
                    command.at(3).at(0) != 'r' && command.at(3).at(0) != 'R' &&
                    command.at(3).at(0) != 'b' && command.at(3).at(0) != 'B' &&
                    command.at(3).at(0) != 'n' && command.at(3).at(0) != 'N')
                        ){
                    // reprompt if pawn promotion is invalid
                    cout << "Invalid move: " << command.at(3) << " is not a valid promotion" << endl;
                    continue;
                }

                move.promote_to = 'Y';
            }

            vector<Move> legal_moves = from_piece->getLegalMoves();

            // re-prompt if not a legal move
            if (find(legal_moves.begin(), legal_moves.end(), move) == legal_moves.end()) {
                cout << "Invalid move: " << command.at(1) << " to " << command.at(2) << (command.size() >= 4 ? " with promotion" : "") << " is not a legal move" << endl;
                continue;
            }

            if (move.promote_to == 'Y') {
                move.promote_to = command.at(3).at(0);
            }

            return move;
        }
        // case for help
        else if (command.front() == "help") {
            cout << "Valid commands:" << endl;
            cout << "\thelp" << endl;
            cout << "\tmove <tile1> <tile2> [promotion]" << endl;
            cout << "\tresign" << endl;
        }
        // case for unknown command
        else {
            cout << "Invalid command: " << command.front() << endl;
        }
    }
}

Move Human::interpretMoveCommand(const vector<string>& command) {
    pair<int, int> from = Grid::positionFromNotation(command.at(1));
    pair<int, int> to = Grid::positionFromNotation(command.at(2));

    if (from == Move::invalid || to == Move::invalid) {
        return {Move::invalid, Move::invalid};
    }

    return {from, to};
}
