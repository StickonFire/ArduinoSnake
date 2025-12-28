#include <vector>

#include "snakeModel.hpp"

Coordinate::Coordinate(int x, int y): x(x), y(y) { }

bool Coordinate::operator==(const Coordinate& other) const{
	return this->x == other.x
		&& this->y == other.y;
}

Snake::Snake(Direction direction,Coordinate headPosition): currentDirection(direction), headPosition(headPosition) { }

bool Snake::operator==(const Snake& other) const {
    return this->currentDirection == other.currentDirection
        && this->headPosition == other.headPosition;
}

SnakeModel::SnakeModel(int mapWidth, int mapHeight, Coordinate startPosition, Direction playerStartDirection): 
  map(std::vector<std::vector<MapTileState>>(mapWidth,std::vector<MapTileState>(mapHeight,Empty))), player(playerStartDirection, startPosition) {
    this->map[startPosition.x][startPosition.y] = SnakeTile;
}
}

}

std::vector<std::vector<MapTileState>> SnakeModel::getMap() {
    return this->map;
}

Snake SnakeModel::getPlayer() {
    return this->player;
}