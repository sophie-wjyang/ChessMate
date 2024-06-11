#include "Utils.h"
#include <string>
#include <sstream>
#include <iostream>

vector<string> splitString(const string& str) {
    vector<string> tokens;
    string token;

    stringstream line_stream{str};

    while (line_stream >> token) {
        tokens.emplace_back(token);
    }

    return tokens;
}
