#include <vector>

#include "snakeModel.hpp"

Snake::Snake(Direction direction,int positionX, int positionY): currentDirection(direction), positionX(positionX), positionY(positionY) { }

bool Snake::operator==(const Snake& other) const {
    return this->currentDirection == other.currentDirection
        && this->positionX == other.positionX
        && this->positionY == other.positionY;
}

SnakeModel::SnakeModel(int mapWidth, int mapHeight, int playerStartX, int playerStartY, Direction playerStartDirection): 
  map(std::vector<std::vector<MapTileState>>(mapWidth,std::vector<MapTileState>(mapHeight,Empty))), player(playerStartDirection, playerStartX, playerStartY) {
    this->map[playerStartX][playerStartY] = SnakeTile;
}

std::vector<std::vector<MapTileState>> SnakeModel::getMap() {
    return this->map;
}

Snake SnakeModel::getPlayer() {
    return this->player;
}