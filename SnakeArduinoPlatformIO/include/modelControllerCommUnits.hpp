#ifndef COMMUNITS
#define COMMUNITS
#include <string>

enum Command{
    LeftTurn, Pause, Straight, RightTurn
};

std::string to_string(Command convert);

enum MapTileState{
    Empty, SnakeTile
};

#endif