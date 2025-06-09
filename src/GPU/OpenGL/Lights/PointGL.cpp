#include <array>
#include <cmath>

#include "Core/Config.hpp"
#include "Core/Math/Mat4.hpp"
#include "Core/Math/Vec3.hpp"
#include "Core/MathConstants.hpp"
#include "GPU/Lights/PointGPU.hpp"
#include "GPU/OpenGL/Lights/PointGL.hpp"
#include "Lighting/PointLight.hpp"

PointLightGL::PointLightGL(const PointLight& light) : PointLightGPU(light) { updateLightSpaceMatrices(); }

void PointLightGL::updateLightSpaceMatrices() {
  static constexpr std::array<lin::Vec3f, CUBE_MAP_FACE_COUNT> faceDirections = {{{1.0F, 0.0F, 0.0F},
                                                                                  {-1.0F, 0.0F, 0.0F},
                                                                                  {0.0F, 1.0F, 0.0F},
                                                                                  {0.0F, -1.0F, 0.0F},
                                                                                  {0.0F, 0.0F, 1.0F},
                                                                                  {0.0F, 0.0F, -1.0F}}};
  static constexpr std::array<lin::Vec3f, CUBE_MAP_FACE_COUNT> upVectors      = {
      {lin::Vec3f(0.0F, -1.0F, 0.0F), lin::Vec3f(0.0F, -1.0F, 0.0F), lin::Vec3f(0.0F, 0.0F, 1.0F),
            lin::Vec3f(0.0F, 0.0F, -1.0F), lin::Vec3f(0.0F, -1.0F, 0.0F), lin::Vec3f(0.0F, -1.0F, 0.0F)}};

  m_far_plane = static_cast<float>(std::sqrt(light()->getIntensity() / MIN_LIGHT_INTENSITY_FAR_PLANE));

  m_lightSpaceMatrices.clear();
  m_lightSpaceMatrices.reserve(CUBE_MAP_FACE_COUNT);

  // 90° field of view for omnidirectional shadow mapping
  const lin::Mat4f lightProjection = lin::Mat4f::Perspective(PI_2, 1.0F, m_near_plane, m_far_plane);

  for(int i = 0; i < CUBE_MAP_FACE_COUNT; ++i) {
    const lin::Mat4f lightView = lin::Mat4f::LookAt(getPosition(), getPosition() + faceDirections[i], upVectors[i]);
    m_lightSpaceMatrices.push_back(lightProjection * lightView);
  }
}