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

class Snake {
    Direction currentDirection;


};

class SnakeModel {
    Snake player;
    
    std::vector<std::vector<MapTileState>> map;

    SnakeModel(int mapWidth, int mapHeight, int playerStartX, int playerStartY);

    /**
     * Advance State does the following:
     * Take the input and make sure the move is legal. If not, kill.
     * Take the input and move the head.
     */
    std::vector<std::vector<MapTileState>> advanceState(Command userInput);
};