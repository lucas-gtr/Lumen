/**
 * @file Config.hpp
 * @brief Configuration file for the rendering engine.
 */
#ifndef CORE_CONFIG_HPP
#define CORE_CONFIG_HPP

#include <cstddef>
#include <string_view>

//<-------- CAMERA --------->
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

static constexpr double DEFAULT_CAMERA_NEAR_PLANE = 0.1;     // in meters
static constexpr double DEFAULT_CAMERA_FAR_PLANE  = 100.0;   // in meters
static constexpr double MIN_CAMERA_PLANE          = 0.01;    // in meters
static constexpr double MAX_CAMERA_PLANE          = 10000.0; // in meters

//<-------- RENDER SETTINGS --------->
static constexpr int DEFAULT_WIDTH = 800; // in pixels
static constexpr int MIN_WIDTH     = 1;
static constexpr int MAX_WIDTH     = 8192;

static constexpr int DEFAULT_HEIGHT = 600; // in pixels
static constexpr int MIN_HEIGHT     = 1;
static constexpr int MAX_HEIGHT     = 8192;

static constexpr int DEFAULT_CHANNEL_COUNT = 3; // RGB
static constexpr int MIN_CHANNEL_COUNT     = 1;
static constexpr int MAX_CHANNEL_COUNT     = 4;

static constexpr int DEFAULT_MAX_BOUNCES = 5; // for path tracing
static constexpr int MIN_MAX_BOUNCES     = 0;
static constexpr int MAX_MAX_BOUNCES     = 100;

static constexpr int DEFAULT_SAMPLES_PER_PIXEL = 1;
static constexpr int MIN_SAMPLES_PER_PIXEL     = 1;
static constexpr int MAX_SAMPLES_PER_PIXEL     = 1024;

//<-------- RENDER EXPORTER --------->
static constexpr std::string_view DEFAULT_FILE_PATH = "RenderImages/";
static constexpr std::string_view DEFAULT_FILE_NAME = "output";

//<-------- SKYBOX --------->
static constexpr double DEFAULT_SKYBOX_COLOR_R = 0.65;
static constexpr double DEFAULT_SKYBOX_COLOR_G = 0.65;
static constexpr double DEFAULT_SKYBOX_COLOR_B = 0.9;

//<-------- TEXTURE --------->
static constexpr double DEFAULT_TEXTURE_UNDEFINED_R = 1.0;
static constexpr double DEFAULT_TEXTURE_UNDEFINED_G = 0.0;
static constexpr double DEFAULT_TEXTURE_UNDEFINED_B = 1.0;

//<-------- LIGHT --------->
static constexpr double DEFAULT_LIGHT_INTENSITY        = 1.0;
static constexpr double MIN_LIGHT_INTENSITY            = 0.0;
static constexpr double MAX_LIGHT_INTENSITY            = 10000.0;
static constexpr double DEFAULT_SPOT_LIGHT_INNER_ANGLE = 10.0;
static constexpr double DEFAULT_SPOT_LIGHT_OUTER_ANGLE = 12.5;
static constexpr double MIN_SPOT_LIGHT_ANGLE           = 0.0;
static constexpr double MAX_SPOT_LIGHT_ANGLE           = 90.0;

//<-------- LIGHT OPENGL --------->
static constexpr float DEFAULT_LIGHT_NEAR_PLANE      = 0.1F;
static constexpr float DEFAULT_LIGHT_FAR_PLANE       = 100.0F;
static constexpr float DIRECTIONAL_LIGHT_DISTANCE    = 20.0F;
static constexpr float ORTHOGRAPHIC_LIGHT_SIZE       = 7.0F;
static constexpr float MIN_LIGHT_INTENSITY_FAR_PLANE = 0.01F;

//<-------- POST-PROCESSING --------->
static constexpr double DEFAULT_TONE_MAPPING_EXPOSURE = 1.0;
static constexpr double MIN_TONE_MAPPING_EXPOSURE     = 0.0;
static constexpr double MAX_TONE_MAPPING_EXPOSURE     = 10.0;

//<-------- BVH --------->
static constexpr double BVH_CONSTRUCTION_EPSILON = 0.0001;

//<-------- RENDER EXECUTION --------->
static constexpr int DEFAULT_BLOCK_SIZE = 256; // in pixels
static constexpr int MIN_BLOCK_SIZE     = 1;
static constexpr int MAX_BLOCK_SIZE     = 1024;

//<-------- MATH --------->
static constexpr double EPSILON = 1e-9; // for floating-point comparisons

//<-------- ALIGNMENT --------->
static constexpr size_t Align8  = 8;
static constexpr size_t Align16 = 16;
static constexpr size_t Align32 = 32;

//<-------- ENGINE --------->
static constexpr float DEFAULT_CAMERA_MOVE_SPEED   = 0.005F;
static constexpr float MIN_CAMERA_MOVE_SPEED       = 0.001F;
static constexpr float MAX_CAMERA_MOVE_SPEED       = 1.0F;
static constexpr float DEFAULT_CAMERA_ROTATE_SPEED = 0.001F;
static constexpr float MIN_CAMERA_ROTATE_SPEED     = 0.0001F;
static constexpr float MAX_CAMERA_ROTATE_SPEED     = 1.0F;
static constexpr float DEFAULT_CAMERA_ZOOM_SPEED   = 1.0F;
static constexpr float MIN_CAMERA_ZOOM_SPEED       = 0.1F;
static constexpr float MAX_CAMERA_ZOOM_SPEED       = 10.0F;
static constexpr float MIN_CAMERA_PITCH            = -89.0F * 3.14F / 180.0F; // in radians
static constexpr float MAX_CAMERA_PITCH            = 89.0F * 3.14F / 180.0F;  // in radians

//<-------- OPENGL --------->
static constexpr int OPENGL_MAJOR_VERSION         = 3;
static constexpr int OPENGL_MINOR_VERSION         = 3;
static constexpr int ERROR_MESSAGE_LENGTH         = 512;
static constexpr int QUAD_INDICES_COUNT           = 6;
static constexpr int LOAD_TEXTURE_UNIT            = 10;
static constexpr int DIFFUSE_TEXTURE_UNIT         = 0;
static constexpr int NORMAL_TEXTURE_UNIT          = 1;
static constexpr int SHADOW_2D_MAP_TEXTURE_UNIT   = 12;
static constexpr int SHADOW_CUBE_MAP_TEXTURE_UNIT = 13;
static constexpr int SCENE_TEXTURE_UNIT           = 14;
static constexpr int SKYBOX_TEXTURE_UNIT          = 15;
static constexpr int CAMERA_UBO_BINDING_POINT     = 0;

//<-------- VERTEX --------->
static constexpr size_t POSITION_LOCATION  = 0;
static constexpr size_t NORMAL_LOCATION    = 1;
static constexpr size_t UV_LOCATION        = 2;
static constexpr size_t TANGENT_LOCATION   = 3;
static constexpr size_t BITANGENT_LOCATION = 4;
static constexpr size_t POSITION_OFFSET    = 0;
static constexpr size_t NORMAL_OFFSET      = 3 * sizeof(float);
static constexpr size_t UV_OFFSET          = 6 * sizeof(float);
static constexpr size_t TANGENT_OFFSET     = 8 * sizeof(float);
static constexpr size_t BITANGENT_OFFSET   = 11 * sizeof(float);
static constexpr size_t VERTEX_SIZE        = 14 * sizeof(float);

//<-------- SHADOW MAP --------->
static constexpr int DEFAULT_SHADOW_MAP_SIZE = 2048; // in pixels
static constexpr int MIN_SHADOW_MAP_SIZE     = 256;
static constexpr int MAX_SHADOW_MAP_SIZE     = 4096;
static constexpr int CUBE_MAP_FACE_COUNT     = 6;

#endif // CORE_CONFIG_HPP