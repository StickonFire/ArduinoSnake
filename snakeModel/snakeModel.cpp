#include <vector>
#include <array>

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
	this->paused = false;
}

SnakeModel::SnakeModel(std::vector<std::vector<MapTileState>> map, Coordinate startPosition, Direction playerStartDirection, bool paused):
  map(map), player(playerStartDirection,startPosition), paused(paused) { 
}

bool SnakeModel::operator==(const SnakeModel& other) const {
    return this->player == other.player
        && this->map == other.map
        && this->paused == other.paused;
}

Direction changeDirection(Direction initial, Command input){
	std::array<Direction,4> clockwiseDirections{Up,Right,Down,Left};
	int position;
	switch(initial){
		case Up:
			position = 0;
			break;
		case Right:
			position = 1;
			break;
		case Down:
			position = 2;
			break;
		case Left:
			position = 3;
	}
	if(input == LeftTurn)
		return clockwiseDirections[(position-1+4)%4];
	if(input == RightTurn)
		return clockwiseDirections[(position+1+4)%4];
	return initial;
}

}

std::vector<std::vector<MapTileState>> SnakeModel::getMap() {
    return this->map;
}

Snake SnakeModel::getPlayer() {
    return this->player;
}

void SnakeModel::togglePause(){
	this->paused = !paused;
}

bool SnakeModel::isPaused() {
	return this->paused;
}