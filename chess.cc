#include <cstdlib>
#include "Game.h"

using namespace std;


int main(int argc, char** argv) {
    int seed;
    if (argc >= 2) {
        seed = ::strtol(argv[1], nullptr, 10);
    }
    else {
        seed = static_cast<unsigned int>(std::time(nullptr));
    }
    srand(seed);

    cout << "Seed: " << seed << endl;

    Game game;
    game.startInterface();

    pair<int, int> final_score = game.getScore();

    cout << endl << "Final score:" << endl;
    cout << "White:" << final_score.first << endl;
    cout << "Black:" << final_score.second << endl;
}
