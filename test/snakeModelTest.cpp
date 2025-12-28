#include <gtest/gtest.h>

#include "snakeModel.hpp"

TEST(SnakeModelTest,SnakeConstructorTest){
    Direction expectedDirection = Left;
    int expectedPositionX = 1;
    int expectedPositionY = 2;

    Snake test(expectedDirection,expectedPositionX,expectedPositionY);

    EXPECT_EQ(test.currentDirection,expectedDirection);
    EXPECT_EQ(test.positionX,expectedPositionX);
    EXPECT_EQ(test.positionY,expectedPositionY);
}

TEST(SnakeModelTest,SnakeModelConstructorTest){
    int mapWidth = 8;
    int mapHeight = 8;
    int playerStartingPositionX = 3;
    int playerStartingPositionY = 4;
    Direction startingDirection = Right;
    SnakeModel test(mapWidth, mapHeight, playerStartingPositionX, playerStartingPositionY, startingDirection);

    Snake expectedSnake(startingDirection,playerStartingPositionX,playerStartingPositionY);
    std::vector<std::vector<MapTileState>> expectedMap{
        std::vector<MapTileState>{Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty},
        std::vector<MapTileState>{Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty},
        std::vector<MapTileState>{Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty},
        std::vector<MapTileState>{Empty,Empty,Empty,Empty,SnakeTile,Empty,Empty,Empty},
        std::vector<MapTileState>{Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty},
        std::vector<MapTileState>{Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty},
        std::vector<MapTileState>{Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty},
        std::vector<MapTileState>{Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty},
    };
    EXPECT_EQ(test.getPlayer(),expectedSnake);
    EXPECT_EQ(test.getMap(),expectedMap);
}