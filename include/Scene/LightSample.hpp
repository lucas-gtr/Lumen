#ifndef RENDERING_LIGHTSAMPLE_HPP
#define RENDERING_LIGHTSAMPLE_HPP

#include "Geometry/Mesh.hpp"
#include <linalg/Vec3.hpp>

#include "Core/CommonTypes.hpp"
#include "Core/Random.hpp"

struct LightSample {
  linalg::Vec3d v1;
  linalg::Vec3d v2;
  linalg::Vec3d v3;
  double        area = 0.0;
  double        intensity;

  linalg::Vec3d random_sample() const {
    double u = randomUniform01();
    double v = randomUniform01();

    if(u + v > 1.0) {
      u = 1.0 - u;
      v = 1.0 - v;
    }

    linalg::Vec3d edge1 = v2 - v1;
    linalg::Vec3d edge2 = v3 - v1;
    return v1 + edge1 * u + edge2 * v;
  }
};

#endif // RENDERING_LIGHTSAMPLE_HPP