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

enum Direction{
    Up, Right, Down, Left
};

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

    bool operator==(const SnakeModel& other) const;

    /**
     * Advance State does the following:
     * Take the input and make sure the move is legal. If not, kill.
     * Take the input and move the head.
     */
    void advanceState(Command userInput);

    std::vector<std::vector<MapTileState>> getMap();
    Snake getPlayer();
    bool isPaused();
};

#endif