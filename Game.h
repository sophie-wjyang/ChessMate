#ifndef CS247_PROJECT_GAME_H
#define CS247_PROJECT_GAME_H

#include "Grid.h"
#include "Player.h"
#include <utility>
#include <memory>
#include <string>

using namespace std;

class Game {
    private:
        Grid grid;

        shared_ptr<Player> white_player;
        shared_ptr<Player> black_player;
        pair<int, int> score;   // first is white, second is black

        static string colourString(Colour colour, bool capitalized);

        shared_ptr<Player> createPlayer(const string& player_type, Colour colour) const;

        bool setPiece(const string& piece, const string& tile);

        bool unsetPiece(const string& tile);

        void setColour(const string& colour);

        bool exitSetup();

        void startSetup();

        void startMatch(const string& p1, const string& p2);

        bool isMated();

        void endMatch(bool is_stalemate);

    public:
        Game();

        void startInterface();

        pair<int, int> getScore();
};


#endif //CS247_PROJECT_GAME_H
