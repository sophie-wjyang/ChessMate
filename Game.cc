#include "Game.h"
#include "Human.h"
#include "CPU1.h"
#include "CPU2.h"
#include "CPU3.h"
#include "CPU4.h"
#include "TextDisplay.h"
#include "GraphicalDisplay.h"
#include <iostream>
#include <memory>
#include <string>
#include "Utils.h"

Game::Game() {
    grid.resetDefault();
    grid.attachDisplay(make_shared<TextDisplay>(grid));
    grid.attachDisplay(make_shared<GraphicalDisplay>(grid));
}

shared_ptr<Player> Game::createPlayer(const string& player_type, Colour colour) const {
    if (player_type == "human") {
        return make_shared<Human>(grid, colour);
    }
    else if (player_type == "computer1") {
        return make_shared<CPU1>(grid, colour);
    }
    else if (player_type == "computer2") {
        return make_shared<CPU2>(grid, colour);
    }
    else if (player_type == "computer3") {
        return make_shared<CPU3>(grid, colour);
    }
    else if (player_type == "computer4") {
        return make_shared<CPU4>(grid, colour);
    }
    else {
        cout << "Invalid player type: " << player_type << endl;
        return nullptr;
    }
}

void Game::startInterface() {
    string line;
    vector<string> command;

    while (true) {
        // cli prompt
        cout << ">> ";

        // exit if read fails
        if (!getline(cin, line)) return;

        // parse command into parts
        command = splitString(line);

        // re-prompt if command is empty
        if (command.empty()) continue;

        // case for starting match
        if (command.front() == "game") {
            // re-prompt if not enough arguments
            if (command.size() < 3) {
                cout << "Usage: game <white-player> <black-player>" << endl;
                continue;
            }

            // start match with specified players
            startMatch(command.at(1), command.at(2));

            // exit if eof reached
            if (!cin) return;
        }
        // case for setup mode
        else if (command.front() == "setup") {
            startSetup();

            // exit if eof reached
            if (!cin) return;
        }
        // case for help
        else if (command.front() == "help") {
            cout << "Valid commands:" << endl;
            cout << "\thelp" << endl;
            cout << "\tgame <white-player> <black-player>:" << endl;
            cout << "\tsetup" << endl;
        }
        // case for unknown command
        else {
            cout << "Invalid command: " << command.front() << endl;
        }
    }
}


#pragma region SETUP MODE

void Game::startSetup() {
    string line;
    vector<string> command;

    // prints initial board
    grid.notifyDisplays();

    while (true) {
        // cli prompt
        cout << ">> ";

        // exit if read fails
        if (!getline(cin, line)) return;

        // parse command into parts
        command = splitString(line);

        // re-prompt if command is empty
        if (command.empty()) continue;

        // case for piece insertion
        if (command.front() == "+") {
            // re-prompt if not enough arguments
            if (command.size() < 3) {
                cout << "Usage: + <piece> <tile>" << endl;
                continue;
            }

            if (!setPiece(command.at(1), command.at(2))) continue;

            grid.notifyDisplays();
        }
        // case for piece removal
        else if (command.front() == "-") {
            // re-prompt if not enough arguments
            if (command.size() < 2) {
                cout << "Usage: - <tile>" << endl;
                continue;
            }

            if (!unsetPiece(command.at(1))) continue;

            grid.notifyDisplays();
        }
        // case for colour change
        else if (command.front() == "=") {
            // re-prompt if not enough arguments
            if (command.size() < 2) {
                cout << "Usage: = <colour>" << endl;
            }

            setColour(command.at(1));
        }
        // case for being done
        else if (command.front() == "done") {
            if (exitSetup()) {
                cout << "Exiting setup mode" << endl;
                return;
            }
        }
        // case for help
        else if (command.front() == "help") {
            cout << "Valid commands:" << endl;
            cout << "\thelp" << endl;
            cout << "\t+ <piece> <tile>" << endl;
            cout << "\t- <tile>" << endl;
            cout << "\t= <colour>" << endl;
            cout << "\tdone" << endl;
        }
        // case for unknown command
        else {
            cout << "Invalid command: " << command.front() << endl;
        }
    }
}

bool Game::setPiece(const string& piece, const string& tile) {
    pair<int, int> position = Grid::positionFromNotation(tile);

    if (position == Move::invalid) return false;

    if (piece.size() != 1 && string("kqrbnpKQRBNP").find(piece) == string::npos) {
        cout << "Invalid piece: " << piece << endl;
        return false;
    }

    auto colour = isupper(piece.at(0)) ? WHITE : BLACK;
    auto piece_id = PieceId(tolower(piece.at(0)));

    grid.getPiece(position) = grid.makePiece(piece_id, colour, position, true);
    return true;
}

bool Game::unsetPiece(const string& tile) {
    pair<int, int> position = Grid::positionFromNotation(tile);

    if (position == Move::invalid) {
        return false;
    }

    grid.getPiece(position) = nullptr;
    return true;
}

void Game::setColour(const string& colour) {
    if (colour == "white") {
        grid.setCurrentPlayer(WHITE);
    }
    else if (colour == "black") {
        grid.setCurrentPlayer(BLACK);
    }
    else {
        cout << "Invalid colour: " << colour << endl;
    }
}

bool Game::exitSetup() {

    if (grid.resetKingPointers() != make_pair(1, 1)) {
        cout << "Invalid state: Each player must have exactly 1 king" << endl;
        return false;
    }

    if (grid.isInCheck(WHITE) || grid.isInCheck(BLACK)) {
        cout << "Invalid state: Neither player can be in check" << endl;
        return false;
    }

    for (int i = 0; i < 8; ++i) {
        const auto& bottom_rank = grid.getPiece(i, 0);
        const auto& top_rank = grid.getPiece(i, 7);

        if ((top_rank && top_rank->getId() == PAWN) || (bottom_rank && bottom_rank->getId() == PAWN)) {
            cout << "Invalid state: There can be no pawns in the top/bottom back rank" << endl;
            return false;
        }
    }

    return true;
}

#pragma endregion


void Game::startMatch(const string& p1, const string& p2) {

    // set white player
    white_player = createPlayer(p1, WHITE);

    // set black player
    black_player = createPlayer(p2, BLACK);

    // quit if either is invalid
    if (!(white_player && black_player)) return;

    while (true) {
        grid.notifyDisplays();

        // if it's been 50 moves since the last capture or pawn move, the game is a draw
        if(grid.getMovesSinceLastPawnMoveOrCapture() >= 100){
            cout << "No pawns have been moved nor captures been made in 50 turns" << endl;
            endMatch(true);
            break;
        }

        if (isMated()) {
            bool is_stalemate = !grid.isInCheck(grid.getCurrentPlayer());

            if (!is_stalemate) {
                cout << colourString(grid.getCurrentPlayer(), true) << " has been checkmated" << endl;
            }

            endMatch(is_stalemate);
            break;
        }

        Move next_move;

        if (grid.getCurrentPlayer() == WHITE) {
            next_move = white_player->getMove();
        }
        else {
            next_move = black_player->getMove();
        }

        // exit if eof reached
        if (!cin) {
            return;
        }

        if (next_move.from == Move::invalid) {
            cout << colourString(grid.getCurrentPlayer(), true) << " player resigns" << endl;
            endMatch(false);
            break;
        }

        grid.makeMove(next_move);
    }
}

bool Game::isMated() {
    return grid.getAllLegalMoves(grid.getCurrentPlayer()).empty();
}

void Game::endMatch(bool is_stalemate) {
    if (is_stalemate) {
        cout << "Stalemate after " << (grid.getTurn() + 1) / 2 << " turns!" << endl;
    }
    else {
        cout << colourString((grid.getCurrentPlayer() == WHITE ? BLACK : WHITE), true)
             << " wins the match after " << (grid.getTurn() + 1) / 2 <<  " turns!" << endl;

        if (grid.getCurrentPlayer() == WHITE) {
            score.second++;
        }
        else {
            score.first++;
        }
    }
    grid.resetDefault();
}


string Game::colourString(Colour colour, bool capitalized) {
    string str;

    if (colour == WHITE) {
        str = "white";
    }
    else {
        str = "black";
    }

    if (capitalized) {
        str.at(0) = toupper(str.at(0));
    }

    return str;
}


pair<int, int> Game::getScore() {
    return score;
}


