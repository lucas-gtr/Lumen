#include <gtest/gtest.h>
#include <linalg/linalg.hpp>
#include "Scene/LightSample.hpp"

TEST(LightSample, DefaultValuesAreCorrect) {
  LightSample sample;
  EXPECT_EQ(sample.area, 0.0);
  EXPECT_EQ(sample.intensity, 0.0);
  EXPECT_EQ(sample.v1, linalg::Vec3d(0.0, 0.0, 0.0));
  EXPECT_EQ(sample.v2, linalg::Vec3d(0.0, 0.0, 0.0));
  EXPECT_EQ(sample.v3, linalg::Vec3d(0.0, 0.0, 0.0));
}

TEST(LightSample, RandomSampleLiesInTriangle) {
  LightSample sample;
  sample.v1 = linalg::Vec3d(0.0, 0.0, 0.0);
  sample.v2 = linalg::Vec3d(1.0, 0.0, 0.0);
  sample.v3 = linalg::Vec3d(0.0, 1.0, 0.0);

  for (int i = 0; i < 1000; ++i) {
    linalg::Vec3d p = sample.randomSample();

    EXPECT_DOUBLE_EQ(p.z, 0.0);

    double u = p.x;
    double v = p.y;
    double w = 1.0 - u - v;

    EXPECT_GE(u, 0.0);
    EXPECT_GE(v, 0.0);
    EXPECT_GE(w, 0.0);
    EXPECT_LE(u, 1.0);
    EXPECT_LE(v, 1.0);
    EXPECT_LE(w, 1.0);
  }
}

TEST(LightSample, RandomSampleIsConsistentWithEdges) {
  LightSample sample;
  sample.v1 = linalg::Vec3d(1.0, 2.0, 3.0);
  sample.v2 = linalg::Vec3d(4.0, 2.0, 3.0);
  sample.v3 = linalg::Vec3d(1.0, 5.0, 3.0);

  for (int i = 0; i < 1000; ++i) {
    linalg::Vec3d p = sample.randomSample();

    // p should lie in the same plane z = 3.0
    EXPECT_DOUBLE_EQ(p.z, 3.0);

    linalg::Vec3d v0 = sample.v2 - sample.v1;
    linalg::Vec3d v1 = sample.v3 - sample.v1;
    linalg::Vec3d v2 = p - sample.v1;

    double d00 = linalg::dot(v0, v0);
    double d01 = linalg::dot(v0, v1);
    double d11 = linalg::dot(v1, v1);
    double d20 = linalg::dot(v2, v0);
    double d21 = linalg::dot(v2, v1);

    double denom = d00 * d11 - d01 * d01;
    double a = (d11 * d20 - d01 * d21) / denom;
    double b = (d00 * d21 - d01 * d20) / denom;
    double c = 1.0 - a - b;

    EXPECT_GE(a, 0.0);
    EXPECT_GE(b, 0.0);
    EXPECT_GE(c, 0.0);
    EXPECT_LE(a, 1.0);
    EXPECT_LE(b, 1.0);
    EXPECT_LE(c, 1.0);
  }
}
