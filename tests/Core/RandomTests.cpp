#include "Core/Random.hpp"
#include <gtest/gtest.h>

TEST(RandomTest, PointsAreInsideUnitDisk) {
    for (int i = 0; i < 1000; ++i) {
        linalg::Vec2d point = randomPointInUnitDisk();
        EXPECT_LE(point.squaredLength(), 1.0);
    }
}

TEST(RandomTest, GeneratesDifferentPoints) {
    linalg::Vec2d p1 = randomPointInUnitDisk();
    linalg::Vec2d p2 = randomPointInUnitDisk();
    linalg::Vec2d p3 = randomPointInUnitDisk();
    bool different = (p1 != p2) || (p2 != p3) || (p1 != p3);
    EXPECT_TRUE(different);
}
