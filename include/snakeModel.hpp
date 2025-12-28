#ifndef SNAKEMODEL
#define SNAKEMODEL

#include <vector>

enum Command{
    LeftTurn, Pause, RightTurn
};

enum Direction{
    Up, Right, Down, Left
};

enum MapTileState{
    Empty, SnakeTile
};

struct Snake {
    Direction currentDirection;
    int positionX;
    int positionY;

    Snake(Direction currentDirection, int positionX, int positionY);
    bool operator==(const Snake& other) const;
};

class SnakeModel {
    Snake player;

    std::vector<std::vector<MapTileState>> map;

    public:
    SnakeModel(int mapWidth, int mapHeight, int playerStartX, int playerStartY, Direction playerStartDirection);

    /**
     * Advance State does the following:
     * Take the input and make sure the move is legal. If not, kill.
     * Take the input and move the head.
     */
    std::vector<std::vector<MapTileState>> advanceState(Command userInput);

    std::vector<std::vector<MapTileState>> getMap();
    Snake getPlayer();

};

#endif