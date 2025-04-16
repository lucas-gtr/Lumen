/**
 * @file Config.hpp
 * @brief Configuration file for the rendering engine.
 */
#ifndef CORE_CONFIG_HPP
#define CORE_CONFIG_HPP

#include <cstddef>
#include <string_view>

//<-------- CAMERA CLASS --------->
static constexpr double DEFAULT_HORIZONTAL_FOV = 45.0; // in degrees
static constexpr double MIN_HORIZONTAL_FOV     = 30.0;
static constexpr double MAX_HORIZONTAL_FOV     = 120.0;

static constexpr double DEFAULT_FOCAL_LENGTH = 50.0; // in mm
static constexpr double MIN_FOCAL_LENGTH     = 10.0;
static constexpr double MAX_FOCAL_LENGTH     = 800.0;

static constexpr double DEFAULT_APERTURE = 2.8; // f-stop
static constexpr double MIN_APERTURE     = 1.0;
static constexpr double MAX_APERTURE     = 22.0;

static constexpr double DEFAULT_SHUTTER_SPEED = 1.0 / 60.0; // in seconds
static constexpr double MIN_SHUTTER_SPEED     = 1.0 / 8000.0;
static constexpr double MAX_SHUTTER_SPEED     = 1.0;

static constexpr double DEFAULT_FOCUS_DISTANCE = 2.0; // in meters
static constexpr double MIN_FOCUS_DISTANCE     = 0.3;
static constexpr double MAX_FOCUS_DISTANCE     = 10000.0;

//<-------- RENDER SETTINGS --------->
static constexpr size_t DEFAULT_WIDTH = 800; // in pixels
static constexpr size_t MIN_WIDTH     = 1;
static constexpr size_t MAX_WIDTH     = 8192;

static constexpr size_t DEFAULT_HEIGHT = 600; // in pixels
static constexpr size_t MIN_HEIGHT     = 1;
static constexpr size_t MAX_HEIGHT     = 8192;

static constexpr size_t DEFAULT_CHANNEL_COUNT = 3; // RGB
static constexpr size_t MIN_CHANNEL_COUNT     = 1;
static constexpr size_t MAX_CHANNEL_COUNT     = 4;

static constexpr double DEFAULT_NEAR_PLANE = 0.1;  // in meters
static constexpr double MIN_NEAR_PLANE     = 0.01; // in meters
static constexpr double MAX_NEAR_PLANE     = 10.0; // in meters

static constexpr double DEFAULT_FAR_PLANE = 1000.0;  // in meters
static constexpr double MIN_FAR_PLANE     = 1.0;     // in meters
static constexpr double MAX_FAR_PLANE     = 10000.0; // in meters

static constexpr int DEFAULT_MAX_BOUNCES = 5; // for path tracing
static constexpr int MIN_MAX_BOUNCES     = 0;
static constexpr int MAX_MAX_BOUNCES     = 100;

static constexpr int DEFAULT_SAMPLES_PER_PIXEL = 1;
static constexpr int MIN_SAMPLES_PER_PIXEL     = 1;
static constexpr int MAX_SAMPLES_PER_PIXEL     = 1024;

//<-------- RENDER EXPORTER --------->
static constexpr std::string_view DEFAULT_FILE_PATH = "RenderImages/";
static constexpr std::string_view DEFAULT_FILE_NAME = "output";

#endif // CORE_CONFIG_HPP