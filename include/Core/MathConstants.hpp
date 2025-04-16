/**
 * @file MathConstants.hpp
 * @brief Header file for mathematical constants used in the project.
 */
#ifndef CORE_MATHCONSTANTS_HPP
#define CORE_MATHCONSTANTS_HPP

#include <numbers>

constexpr double ANGLE_MAX_DEGREES    = 180.0;
constexpr double DEG_TO_RAD           = std::numbers::pi / ANGLE_MAX_DEGREES;
constexpr double RAD_TO_DEG           = ANGLE_MAX_DEGREES / std::numbers::pi;
constexpr double MM_TO_M              = 0.001;
constexpr double M_TO_MM              = 1000.0;
constexpr double HALF                 = 0.5;
constexpr double NORMALIZED_TO_COLOR8 = 255.999;

#endif // CORE_MATHCONSTANTS_HPP