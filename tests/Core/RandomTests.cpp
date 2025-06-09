#include "Core/Random.hpp"
#include <gtest/gtest.h>

TEST(RandomTest, PointsAreInsideUnitDisk) {
    for (int i = 0; i < 1000; ++i) {
        lin::Vec2d point = randomPointInUnitDisk();
        EXPECT_LE(point.squaredLength(), 1.0);
    }
}

TEST(RandomTest, GeneratesDifferentPoints) {
    lin::Vec2d p1 = randomPointInUnitDisk();
    lin::Vec2d p2 = randomPointInUnitDisk();
    lin::Vec2d p3 = randomPointInUnitDisk();
    bool different = (p1 != p2) || (p2 != p3) || (p1 != p3);
    EXPECT_TRUE(different);
}
