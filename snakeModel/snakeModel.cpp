#include <vector>
#include <array>
#include <string>

#include "snakeModel.hpp"

std::string to_string(Command convert){
	switch(convert){
		case LeftTurn:
			return "LeftTurn";
		case Pause:
			return "Pause";
		case Straight:
			return "Straight";
		case RightTurn:
			return "RightTurn";
	}
}

std::string to_string(Direction convert){
	switch(convert){
		case Up:
			return "Up";
		case Right:
			return "Right";
		case Down:
			return "Down";
		case Left:
			return "Left";
	}
}

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

void SnakeModel::advanceState(Command userInput){
	if(userInput == Pause){
		togglePause();
		return;
	}
	if(!paused){
		player.currentDirection = changeDirection(player.currentDirection,userInput);
		Coordinate nextPosition(player.headPosition);
		switch(player.currentDirection){
			case Up:
				nextPosition.x--;
				break;
			case Right:
				nextPosition.y++;
				break;
			case Down:
				nextPosition.x++;
				break;
			case Left:
				nextPosition.y--;
		}

		if(nextPosition.x >= map.size() || nextPosition.x < 0 || nextPosition.y >= map[0].size() || nextPosition.y < 0)
			return;
		
		map[player.headPosition.x][player.headPosition.y] = Empty;
		map[nextPosition.x][nextPosition.y] = SnakeTile;
		
		player.headPosition = nextPosition;
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