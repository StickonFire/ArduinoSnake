#ifndef SNAKEMODEL
#define SNAKEMODEL

#include "hardwareValues.hpp"
#include "modelControllerCommUnits.hpp"
#include "constrainedVector.hpp"

struct Coordinate{
    int x;
    int y;

    Coordinate(int x, int y);
    bool operator==(const Coordinate& other) const;
};

enum Direction{
    Up, Right, Down, Left
};

std::string to_string(Direction convert);

Direction changeDirection(Direction direction,Command toChange);

struct Snake {
    Direction currentDirection;
    Coordinate headPosition;

    Snake(Direction currentDirection, Coordinate headPosition);
    bool operator==(const Snake& other) const;
};

class SnakeModel {
    Snake player;
    bool paused;
    ConstrainedVector<ConstrainedVector<MapTileState,ledCols>,ledRows> map;

    public:
    SnakeModel(int mapWidth, int mapHeight, Coordinate startPosition, Direction playerStartDirection);
    SnakeModel(ConstrainedVector<ConstrainedVector<MapTileState,ledCols>,ledRows> map, Coordinate startPosition, Direction playerStartDirection, bool paused);

    bool operator==(const SnakeModel& other) const;

    /**
     * Advance State does the following:
     * Take the input and make sure the move is legal. If not, kill.
     * Take the input and move the head.
     */
    void advanceState(Command userInput);

    ConstrainedVector<ConstrainedVector<MapTileState,ledCols>,ledRows> getMap();
    Snake getPlayer();
    void togglePause();
    bool isPaused();
};

#endif