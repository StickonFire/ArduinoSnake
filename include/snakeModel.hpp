#ifndef SNAKEMODEL
#define SNAKEMODEL

#include <vector>

struct Coordinate{
    int x;
    int y;

    Coordinate(int x, int y);
    bool operator==(const Coordinate& other) const;
};

enum Command{
    LeftTurn, Pause, Straight, RightTurn
};

std::string to_string(Command convert);

enum Direction{
    Up, Right, Down, Left
};

std::string to_string(Direction convert);

Direction changeDirection(Direction direction,Command toChange);

enum MapTileState{
    Empty, SnakeTile
};

struct Snake {
    Direction currentDirection;
    Coordinate headPosition;

    Snake(Direction currentDirection, Coordinate headPosition);
    bool operator==(const Snake& other) const;
};

class SnakeModel {
    Snake player;
    bool paused;
    std::vector<std::vector<MapTileState>> map;

    public:
    SnakeModel(int mapWidth, int mapHeight, Coordinate startPosition, Direction playerStartDirection);
    SnakeModel(std::vector<std::vector<MapTileState>> map, Coordinate startPosition, Direction playerStartDirection, bool paused);

    bool operator==(const SnakeModel& other) const;

    /**
     * Advance State does the following:
     * Take the input and make sure the move is legal. If not, kill.
     * Take the input and move the head.
     */
    void advanceState(Command userInput);

    std::vector<std::vector<MapTileState>> getMap();
    Snake getPlayer();
    void togglePause();
    bool isPaused();
};

#endif