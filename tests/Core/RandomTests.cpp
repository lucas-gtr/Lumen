#include "Core/Random.hpp"
#include <gtest/gtest.h>

TEST(RandomTest, PointsAreInsideUnitDisk) {
    for (int i = 0; i < 1000; ++i) {
        lin::Vec2 point = randomPointInUnitDisk();
        EXPECT_LE(point.squaredLength(), 1.0);
    }
}

TEST(RandomTest, GeneratesDifferentPoints) {
    lin::Vec2 p1 = randomPointInUnitDisk();
    lin::Vec2 p2 = randomPointInUnitDisk();
    lin::Vec2 p3 = randomPointInUnitDisk();
    bool different = (p1 != p2) || (p2 != p3) || (p1 != p3);
    EXPECT_TRUE(different);
}
