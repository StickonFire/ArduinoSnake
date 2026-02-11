#include <vector>
#include <array>
#include <string>

#include "modelControllerCommUnits.hpp"
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
		default:
			return "Not Implemented";
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
		default:
			return "Not Implemented";
	}
}

Coordinate::Coordinate(int x, int y): x(x), y(y) { }

Coordinate::Coordinate(Coordinate original, Direction direction): x(original.x), y(original.y) {
	switch(direction){
		case Up:
			this->x--;
			break;
		case Right:
			this->y++;
			break;
		case Down:
			this->x++;
			break;
		case Left:
			this->y--;
	}
}

bool Coordinate::operator==(const Coordinate& other) const{
	return this->x == other.x
		&& this->y == other.y;
}

Snake::Snake(Direction direction,Coordinate headPosition): currentDirection(direction), headPosition(headPosition), tailPosition(headPosition), nodeDirection{direction} { }

Snake::Snake(Coordinate headPosition, Coordinate tailPosition, ConstrainedVector<Direction,ledNums> nodeDirection):
	currentDirection(currentDirection), headPosition(headPosition), tailPosition(tailPosition), nodeDirection{nodeDirection} { }

bool Snake::operator==(const Snake& other) const {
    return this->headPosition == other.headPosition
		&& this->tailPosition == other.tailPosition
		&& this->nodeDirection == other.nodeDirection;
}

bool Snake::moveHead(Direction next){
	if(!(this->nodeDirection.push_back(next)))
		return false;
	this->headPosition = Coordinate(this->headPosition,next);
	return true;
}

bool Snake::moveTail(){
	if(this->nodeDirection.size() < 2)
		return false;
	this->nodeDirection.pop_front();
	this->tailPosition = Coordinate(this->tailPosition,nodeDirection[0]);
	return true;
}

SnakeModel::SnakeModel(): player(Right,Coordinate(ledRows/2,ledCols/2)), map(ledRows,ConstrainedVector<MapTileState,ledCols>(ledCols,Empty)), paused(false), killed(false) {}

SnakeModel::SnakeModel(int mapWidth, int mapHeight, Coordinate startPosition, Direction playerStartDirection, bool paused, bool killed): 
  map(ConstrainedVector<ConstrainedVector<MapTileState,ledCols>,ledRows>(mapWidth,ConstrainedVector<MapTileState,ledCols>(mapHeight,Empty))), player(playerStartDirection, startPosition), paused(paused), killed(killed) {
    this->map[startPosition.x][startPosition.y] = SnakeTile;
}

SnakeModel::SnakeModel(ConstrainedVector<ConstrainedVector<MapTileState,ledCols>,ledRows> map, Coordinate startPosition, Direction playerStartDirection, bool paused, bool killed):
  map(map), player(playerStartDirection,startPosition), paused(paused), killed(killed) { 
}

SnakeModel::SnakeModel(ConstrainedVector<ConstrainedVector<MapTileState,ledCols>,ledRows> map, Snake player, bool paused, bool killed): map(map), player(player), paused(paused), killed(killed) { }

bool SnakeModel::operator==(const SnakeModel& other) const {
    return this->player == other.player
        && this->map == other.map
        && this->paused == other.paused
		&& this->killed == other.killed;
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
		Direction nextDirection = changeDirection(player.nodeDirection[player.nodeDirection.size()-1],userInput);
		Coordinate nextPosition(player.headPosition,nextDirection);

		if((nextPosition.x >= map.size() || nextPosition.x < 0 || nextPosition.y >= map[0].size() || nextPosition.y < 0)
			|| (map[nextPosition.x][nextPosition.y] == SnakeTile && !(nextPosition == player.tailPosition))){
			killed = true;
			return;
		}


		player.moveHead(nextDirection);
		if(map[nextPosition.x][nextPosition.y] != Apple){
			map[player.tailPosition.x][player.tailPosition.y] = Empty;
			player.moveTail();
		}
		map[nextPosition.x][nextPosition.y] = SnakeTile;
	}
}

ConstrainedVector<ConstrainedVector<MapTileState,ledCols>,ledRows> SnakeModel::getMap() {
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

void SnakeModel::setKilled(bool killed){
	this->killed = killed;
}

bool SnakeModel::isKilled() {
	return this->killed;
}