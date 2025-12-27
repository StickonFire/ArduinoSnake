#include <vector>

enum Command{
    Left, Pause, Right
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
};

class SnakeModel {
    Snake player;

    std::vector<std::vector<MapTileState>> map;

    SnakeModel(int mapWidth, int mapHeight, int playerStartX, int playerStartY, Direction playerStartDirection);

    /**
     * Advance State does the following:
     * Take the input and make sure the move is legal. If not, kill.
     * Take the input and move the head.
     */
    std::vector<std::vector<MapTileState>> advanceState(Command userInput);
};