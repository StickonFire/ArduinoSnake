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
    ConstrainedVector<MapTileState,3> differsByFront(2,std::array<MapTileState,3>{SnakeTile,Empty,Empty},1,2);
    EXPECT_FALSE(comparisonPoint == differsByFront);
    ConstrainedVector<MapTileState,3> differsByBack(2,std::array<MapTileState,3>{SnakeTile,Empty,Empty},0,3);
    EXPECT_FALSE(comparisonPoint == differsByBack);
}

TEST(ConstrainedVectorEqualityTest,OffsetInArray){
    ConstrainedVector<MapTileState,3> comparisonPoint(1,std::array<MapTileState,3>{SnakeTile});
    ConstrainedVector<MapTileState,3> shouldUnequal(1,std::array<MapTileState,3>{Empty,SnakeTile},1,2);
    EXPECT_FALSE(comparisonPoint == shouldUnequal);
}

TEST(ConstrainedVectorFillConstructorTest,size0){
    ConstrainedVector<MapTileState,10> test(0,SnakeTile);
    ConstrainedVector<MapTileState,10> expected(0,std::array<MapTileState,10>(),0,0);
    EXPECT_EQ(test,expected);
    EXPECT_EQ(test.size(),0);
}

TEST(ConstrainedVectorFillConstructorTest,sizeEqualsCapacity){
    ConstrainedVector<MapTileState,4> test(4,SnakeTile);
    ConstrainedVector<MapTileState,4> expected(4,std::array<MapTileState,4>{SnakeTile,SnakeTile,SnakeTile,SnakeTile},0,4);
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
    ConstrainedVector<MapTileState,10> expected(5,std::array<MapTileState,10>{SnakeTile,Empty,SnakeTile,Empty,SnakeTile,Empty,Empty,Empty,Empty,Empty},0,5);
    EXPECT_EQ(test,expected);
}

TEST(ConstrainedVectorInitializerTest,EqualsConstraint){
    ConstrainedVector<MapTileState,5> test{SnakeTile,Empty,SnakeTile,Empty,SnakeTile};
    ConstrainedVector<MapTileState,5> expected(5,std::array<MapTileState,5>{SnakeTile,Empty,SnakeTile,Empty,SnakeTile},0,5);
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

TEST(ConstrainedVectorGetOperatorTest,OffsetNoOverflow){
    ConstrainedVector<int,5> test(4,std::array<int,5>{0,1,2,3,4},3,2);
    int expected = 4;
    EXPECT_EQ(test[1],expected);
    test[0] = 0;
    ConstrainedVector<int,5> expectedVector(4,std::array<int,5>{0,1,2,0,4},3,2);
    EXPECT_EQ(test,expectedVector);
}

TEST(ConstrainedVectorGetOperatorTest,OffsetWithOverflow){
    ConstrainedVector<int,5> test(4,std::array<int,5>{0,1,2,3,4},3,2);
    int expected = 1;
    EXPECT_EQ(test[3],expected);
    test[2] = 2;
    ConstrainedVector<int,5> expectedVector(4,std::array<int,5>{2,1,2,3,4},3,2);
    EXPECT_EQ(test,expectedVector);
}

TEST(ConstrainedVectorGetOperatorTest,BetweenSizeAndCapacity){
    ConstrainedVector<MapTileState,5> test{SnakeTile,SnakeTile,SnakeTile};
    EXPECT_THROW(test[3],std::invalid_argument);
    EXPECT_THROW(test[4] = Empty,std::invalid_argument);
}

TEST(ConstrainedVectorGetOperatorTest,BetweenSizeAndCapacityWithOffset){
    ConstrainedVector<int,5> test(3,std::array<int,5>{0,1,2,3,4},1,4);
    EXPECT_THROW(test[3],std::invalid_argument);
    EXPECT_THROW(test[3] = Empty,std::invalid_argument);
}

class ConstrainedVectorPushTest : public testing::Test {
    protected:
        void pushTest(ConstrainedVector<MapTileState,4> &test, ConstrainedVector<MapTileState,4> &expected, MapTileState toPush,std::string message){
            EXPECT_TRUE(test.push_back(toPush)) << message;
            EXPECT_EQ(test,expected) << message;
        }
};

TEST_F(ConstrainedVectorPushTest,EmptyNoOverflow){
    ConstrainedVector<MapTileState,4> test(0,Empty);
    ConstrainedVector<MapTileState,4> expected(1,{SnakeTile,Empty,Empty,Empty},0,1);
    pushTest(test,expected,SnakeTile,"Push fails when the ConstrainedVector's Empty");
}

TEST_F(ConstrainedVectorPushTest,AlmostFullNoOverflow){
    ConstrainedVector<MapTileState,4> test(3,{SnakeTile,SnakeTile,SnakeTile,Empty},0,3);
    ConstrainedVector<MapTileState,4> expected(4,{SnakeTile,SnakeTile,SnakeTile,SnakeTile},0,0);
    pushTest(test,expected,SnakeTile,"Push fails when the ConstrainedVector's Almost Full");
}

TEST_F(ConstrainedVectorPushTest,Full){
    ConstrainedVector<MapTileState,4> test(4,{Empty,Empty,Empty,Empty},0,0);
    EXPECT_FALSE(test.push_back(SnakeTile));
}

TEST_F(ConstrainedVectorPushTest,EmptyOverflow){
    ConstrainedVector<MapTileState,4> test(0,{Empty,Empty,Empty,Empty},3,3);
    ConstrainedVector<MapTileState,4> expected(1,{Empty,Empty,Empty,Empty},3,0);
    pushTest(test,expected,SnakeTile,"Push fails when the ConstrainedVector's Empty and overflows");
}

TEST_F(ConstrainedVectorPushTest,AlmostFullOverflow){
    ConstrainedVector<MapTileState,4> test(1,{Empty,SnakeTile,SnakeTile,SnakeTile},1,0);
    ConstrainedVector<MapTileState,4> expected(2,{SnakeTile,SnakeTile,SnakeTile,SnakeTile},1,1);
    pushTest(test,expected,SnakeTile,"Push fails when the ConstrainedVector's near overflow, but empty");
}

class ConstrainedVectorPopTest : public testing::Test {
    protected:
        void pushTest(ConstrainedVector<MapTileState,4> &test, ConstrainedVector<MapTileState,4> &expected,std::string message){
            EXPECT_TRUE(test.pop_front()) << message;
            EXPECT_EQ(test,expected) << message;
        }
};

TEST_F(ConstrainedVectorPopTest,FullNoOverflow){
    ConstrainedVector<MapTileState,4> test(4,std::array<MapTileState,4>{SnakeTile,SnakeTile,SnakeTile,SnakeTile},1,1);
    ConstrainedVector<MapTileState,4> expected(3,std::array<MapTileState,4>{SnakeTile,SnakeTile,SnakeTile,SnakeTile},2,1);
    pushTest(test,expected,"Pop fails when the ConstrainedVector's Full");
}

TEST_F(ConstrainedVectorPopTest,AlmostEmptyNoOverflow){
    ConstrainedVector<MapTileState,4> test(1,std::array<MapTileState,4>{Empty,Empty,Empty,Empty},0,1);
    ConstrainedVector<MapTileState,4> expected(0,std::array<MapTileState,4>{Empty,Empty,Empty,Empty},1,1);
    pushTest(test,expected,"Pop fails when the ConstrainedVector's Almost Empty");
}

TEST_F(ConstrainedVectorPopTest,EmptyVector){
    ConstrainedVector<MapTileState,4> test(0,{Empty,Empty,Empty,Empty},0,0);
    EXPECT_FALSE(test.pop_front());
}

TEST_F(ConstrainedVectorPopTest,FullOverflow){
    ConstrainedVector<MapTileState,4> test(4,std::array<MapTileState,4>{Empty,Empty,Empty,Empty},3,3);
    ConstrainedVector<MapTileState,4> expected(3,std::array<MapTileState,4>{Empty,Empty,Empty,Empty},0,3);
    pushTest(test,expected,"pop fails when the ConstrainedVector's full and overflows");
}

TEST_F(ConstrainedVectorPopTest,AlmostEmptyOverflow){
    ConstrainedVector<MapTileState,4> test(1,std::array<MapTileState,4>{Empty,Empty,Empty,Empty},3,0);
    ConstrainedVector<MapTileState,4> expected(0,std::array<MapTileState,4>{Empty,Empty,Empty,Empty},0,0);
    pushTest(test,expected,"Pop fails when the ConstrainedVector's near overflow and near empty");
}