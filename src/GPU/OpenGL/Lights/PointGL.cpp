#include <array>
#include <cmath>
#include <linalg/Mat4.hpp>
#include <linalg/Vec3.hpp>

#include "Core/Config.hpp"
#include "Core/MathConstants.hpp"
#include "GPU/Lights/PointGPU.hpp"
#include "GPU/OpenGL/Lights/PointGL.hpp"
#include "Lighting/PointLight.hpp"

PointLightGL::PointLightGL(PointLight* light) : PointLightGPU(light) {
  PointLightGL::updateLightSpaceMatrix();
  light->getTransformationChangedObserver().add([this]() {
    retrieveData();
    updateLightSpaceMatrix();
  });
  light->getLightChangedObserver().add([this]() {
    retrieveData();
    updateLightSpaceMatrix();
  });
}

void PointLightGL::updateLightSpaceMatrix() {
  static constexpr std::array<linalg::Vec3f, CUBE_MAP_FACE_COUNT> face_directions = {{{1.0F, 0.0F, 0.0F},
                                                                                      {-1.0F, 0.0F, 0.0F},
                                                                                      {0.0F, 1.0F, 0.0F},
                                                                                      {0.0F, -1.0F, 0.0F},
                                                                                      {0.0F, 0.0F, 1.0F},
                                                                                      {0.0F, 0.0F, -1.0F}}};
  static constexpr std::array<linalg::Vec3f, CUBE_MAP_FACE_COUNT> up_vectors      = {
      {linalg::Vec3f(0.0F, -1.0F, 0.0F), linalg::Vec3f(0.0F, -1.0F, 0.0F), linalg::Vec3f(0.0F, 0.0F, 1.0F),
            linalg::Vec3f(0.0F, 0.0F, -1.0F), linalg::Vec3f(0.0F, -1.0F, 0.0F), linalg::Vec3f(0.0F, -1.0F, 0.0F)}};

  setFarPlane(static_cast<float>(std::sqrt(light()->getIntensity() / MIN_LIGHT_INTENSITY_FAR_PLANE)));

  m_light_space_matrices.clear();
  m_light_space_matrices.reserve(CUBE_MAP_FACE_COUNT);

  // 90° field of view for omnidirectional shadow mapping
  const linalg::Mat4f light_projection = linalg::Mat4f::Perspective(PI_2, 1.0F, getNearPlane(), getFarPlane());

  for(int i = 0; i < CUBE_MAP_FACE_COUNT; ++i) {
    const linalg::Mat4f light_view =
        linalg::Mat4f::LookAt(getPosition(), getPosition() + face_directions[i], up_vectors[i]);
    m_light_space_matrices.push_back(light_projection * light_view);
  }
}