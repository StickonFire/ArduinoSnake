#include <array>
#include <stdexcept>

#include <gtest/gtest.h>

#include "constrainedVector.hpp"
#include "modelControllerCommUnits.hpp"

TEST(ConstrainedVectorEqualityTest,Equals){
    ConstrainedVector<MapTileState,10> first(5,std::array<MapTileState,10>{SnakeTile,SnakeTile,SnakeTile,SnakeTile,SnakeTile,Empty,Empty,Empty,Empty,Empty});
    ConstrainedVector<MapTileState,10> second(5,std::array<MapTileState,10>{SnakeTile,SnakeTile,SnakeTile,SnakeTile,SnakeTile,Empty,Empty,Empty,Empty,Empty});
    EXPECT_EQ(first,second);
}

TEST(ConstrainedVectorEqualityTest,CycleUnequal){
    ConstrainedVector<MapTileState,3> comparisonPoint(2,std::array<MapTileState,3>{SnakeTile,SnakeTile,Empty});
    ConstrainedVector<MapTileState,3> differsBySize(3,std::array<MapTileState,3>{SnakeTile,SnakeTile,Empty});
    EXPECT_FALSE(comparisonPoint == differsBySize);
    ConstrainedVector<MapTileState,3> differsByContent(2,std::array<MapTileState,3>{SnakeTile,Empty,Empty});
    EXPECT_FALSE(comparisonPoint == differsBySize);
}

TEST(ConstrainedVectorFillConstructorTest,size0){
    ConstrainedVector<MapTileState,10> test(0,SnakeTile);
    ConstrainedVector<MapTileState,10> expected(0,std::array<MapTileState,10>());
    EXPECT_EQ(test,expected);
    EXPECT_EQ(test.size(),0);
}

TEST(ConstrainedVectorFillConstructorTest,sizeEqualsCapacity){
    ConstrainedVector<MapTileState,4> test(4,SnakeTile);
    ConstrainedVector<MapTileState,4> expected(4,std::array<MapTileState,4>{SnakeTile,SnakeTile,SnakeTile,SnakeTile});
    EXPECT_EQ(test,expected);
    EXPECT_EQ(test.size(),4);
}

TEST(ConstrainedVectorFillConstructorTest,sizeOverCapacity){
    bool exceptionThrown = false;
    try{
        ConstrainedVector<MapTileState,10> test(11,Empty);
    } catch (std::invalid_argument e){
        exceptionThrown = true;
    }
    EXPECT_TRUE(exceptionThrown);
}

TEST(ConstrainedVectorInitializerTest,UnderConstraint){
    ConstrainedVector<MapTileState,10> test{SnakeTile,Empty,SnakeTile,Empty,SnakeTile};
    ConstrainedVector<MapTileState,10> expected(5,std::array<MapTileState,10>{SnakeTile,Empty,SnakeTile,Empty,SnakeTile,Empty,Empty,Empty,Empty,Empty});
    EXPECT_EQ(test,expected);
}

TEST(ConstrainedVectorInitializerTest,EqualsConstraint){
    ConstrainedVector<MapTileState,5> test{SnakeTile,Empty,SnakeTile,Empty,SnakeTile};
    ConstrainedVector<MapTileState,5> expected(5,std::array<MapTileState,5>{SnakeTile,Empty,SnakeTile,Empty,SnakeTile});
    EXPECT_EQ(test,expected);
}

TEST(ConstrainedVectorInitializerTest,OverConstraint){
    bool exceptionThrown = false;
    try{
        ConstrainedVector<MapTileState,2> test{SnakeTile,Empty,SnakeTile,Empty,SnakeTile};
    } catch (std::invalid_argument e){
        exceptionThrown = true;
    }
    EXPECT_TRUE(exceptionThrown);
}

TEST(ConstrainedVectorGetOperatorTest,FirstElement){
    MapTileState expected = SnakeTile;
    ConstrainedVector<MapTileState,1> test(1,SnakeTile);
    EXPECT_EQ(test[0],expected);
}

TEST(ConstrainedVectorGetOperatorTest,LastElement){
    MapTileState expected = SnakeTile;
    ConstrainedVector<MapTileState,3> test{Empty,Empty,SnakeTile};
    EXPECT_EQ(test[2],expected);
}

TEST(ConstrainedVectorGetOperatorTest,ModifyingAnElement){
    ConstrainedVector<MapTileState,3> test{Empty,Empty,SnakeTile};
    test[0] = SnakeTile;
    EXPECT_EQ(test[0],SnakeTile);
}

TEST(ConstrainedVectorGetOperatorTest,BetweenSizeAndCapacity){
    ConstrainedVector<MapTileState,5> test{SnakeTile,SnakeTile,SnakeTile};
    EXPECT_THROW(test[3],std::invalid_argument);
    EXPECT_THROW(test[4] = Empty,std::invalid_argument);
}