/**
 * @file MathConstants.hpp
 * @brief Header file for mathematical constants used in the project.
 */
#ifndef CORE_MATHCONSTANTS_HPP
#define CORE_MATHCONSTANTS_HPP

#include <numbers>

static constexpr double ANGLE_MAX_DEGREES = 180.0;
static constexpr double DEG_TO_RAD        = std::numbers::pi / ANGLE_MAX_DEGREES;
static constexpr double RAD_TO_DEG        = ANGLE_MAX_DEGREES / std::numbers::pi;

static constexpr double INV_PI  = 1.0 / std::numbers::pi;
static constexpr double INV_2PI = 1.0 / (2.0 * std::numbers::pi);

static constexpr double MM_TO_M = 0.001;
static constexpr double M_TO_MM = 1000.0;

static constexpr double HALF = 0.5;

#endif // CORE_MATHCONSTANTS_HPP