add_library(QtInterface STATIC
  Application.cpp
  MainWindow.cpp
  MaterialSelectorWidget.cpp
  TextureSelectorWidget.cpp
  TexturePreviewWidget.cpp
  View/Light/LightView.cpp
  View/Light/PointLightView.cpp
  View/Light/DirectionalLightView.cpp
  View/Light/SpotLightView.cpp
  Controller/Light/LightController.cpp
  Controller/Light/PointLightController.cpp
  Controller/Light/DirectionalLightController.cpp
  Controller/Light/SpotLightController.cpp
  MeshBuilderDialog/MeshBuilderDialog.cpp
  MeshBuilderDialog/CubeMeshBuilderDialog.cpp
  MeshBuilderDialog/PlaneMeshBuilderDialog.cpp
  MeshBuilderDialog/SphereMeshBuilderDialog.cpp
  View/RealtimeView.cpp
  Controller/RealtimeController.cpp
  View/SceneView.cpp
  Controller/SceneController.cpp
  View/MenuView.cpp
  Controller/MenuController.cpp
  View/TransformView.cpp
  Controller/TransformController.cpp
  View/ObjectView.cpp
  Controller/ObjectController.cpp
  View/CameraView.cpp
  Controller/CameraController.cpp
  View/ViewportView.cpp
  Controller/ViewportController.cpp
  View/RenderView.cpp
  Controller/RenderController.cpp
  View/RenderWindowView.cpp
  View/ExportView.cpp 
  Controller/ExportController.cpp 
  View/TextureView.cpp
  Controller/TextureController.cpp
  View/SkyboxView.cpp
  Controller/SkyboxController.cpp
  View/MaterialView.cpp
  Controller/MaterialController.cpp
  ${PROJECT_SOURCE_DIR}/include/Qt/MaterialSelectorWidget.hpp
  ${PROJECT_SOURCE_DIR}/include/Qt/TextureSelectorWidget.hpp
  ${PROJECT_SOURCE_DIR}/include/Qt/TexturePreviewWidget.hpp
  ${PROJECT_SOURCE_DIR}/include/Qt/OpenGLContext.hpp
  ${PROJECT_SOURCE_DIR}/include/Qt/View/Light/LightView.hpp
  ${PROJECT_SOURCE_DIR}/include/Qt/View/Light/PointLightView.hpp
  ${PROJECT_SOURCE_DIR}/include/Qt/View/Light/DirectionalLightView.hpp
  ${PROJECT_SOURCE_DIR}/include/Qt/View/Light/SpotLightView.hpp
  ${PROJECT_SOURCE_DIR}/include/Qt/Controller/Light/LightController.hpp
  ${PROJECT_SOURCE_DIR}/include/Qt/Controller/Light/PointLightController.hpp
  ${PROJECT_SOURCE_DIR}/include/Qt/Controller/Light/DirectionalLightController.hpp
  ${PROJECT_SOURCE_DIR}/include/Qt/Controller/Light/SpotLightController.hpp
  ${PROJECT_SOURCE_DIR}/include/Qt/MainWindow.hpp
  ${PROJECT_SOURCE_DIR}/include/Qt/MeshBuilderDialog/MeshBuilderDialog.hpp
  ${PROJECT_SOURCE_DIR}/include/Qt/MeshBuilderDialog/CubeMeshBuilderDialog.hpp
  ${PROJECT_SOURCE_DIR}/include/Qt/MeshBuilderDialog/PlaneMeshBuilderDialog.hpp
  ${PROJECT_SOURCE_DIR}/include/Qt/MeshBuilderDialog/SphereMeshBuilderDialog.hpp
  ${PROJECT_SOURCE_DIR}/include/Qt/View/RealtimeView.hpp
  ${PROJECT_SOURCE_DIR}/include/Qt/Controller/RealtimeController.hpp
  ${PROJECT_SOURCE_DIR}/include/Qt/View/SceneView.hpp
  ${PROJECT_SOURCE_DIR}/include/Qt/Controller/SceneController.hpp
  ${PROJECT_SOURCE_DIR}/include/Qt/View/MenuView.hpp
  ${PROJECT_SOURCE_DIR}/include/Qt/Controller/MenuController.hpp
  ${PROJECT_SOURCE_DIR}/include/Qt/View/TransformView.hpp
  ${PROJECT_SOURCE_DIR}/include/Qt/Controller/TransformController.hpp
  ${PROJECT_SOURCE_DIR}/include/Qt/View/ObjectView.hpp
  ${PROJECT_SOURCE_DIR}/include/Qt/Controller/ObjectController.hpp
  ${PROJECT_SOURCE_DIR}/include/Qt/View/CameraView.hpp
  ${PROJECT_SOURCE_DIR}/include/Qt/Controller/CameraController.hpp
  ${PROJECT_SOURCE_DIR}/include/Qt/View/ViewportView.hpp
  ${PROJECT_SOURCE_DIR}/include/Qt/Controller/ViewportController.hpp
  ${PROJECT_SOURCE_DIR}/include/Qt/View/RenderView.hpp
  ${PROJECT_SOURCE_DIR}/include/Qt/Controller/RenderController.hpp
  ${PROJECT_SOURCE_DIR}/include/Qt/View/RenderWindowView.hpp
  ${PROJECT_SOURCE_DIR}/include/Qt/View/ExportView.hpp
  ${PROJECT_SOURCE_DIR}/include/Qt/Controller/ExportController.hpp
  ${PROJECT_SOURCE_DIR}/include/Qt/View/TextureView.hpp
  ${PROJECT_SOURCE_DIR}/include/Qt/Controller/TextureController.hpp
  ${PROJECT_SOURCE_DIR}/include/Qt/View/SkyboxView.hpp
  ${PROJECT_SOURCE_DIR}/include/Qt/Controller/SkyboxController.hpp
  ${PROJECT_SOURCE_DIR}/include/Qt/View/MaterialView.hpp
  ${PROJECT_SOURCE_DIR}/include/Qt/Controller/MaterialController.hpp
)

find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGL OpenGLWidgets)

set_target_properties(QtInterface PROPERTIES
    AUTOMOC ON
    AUTORCC ON
    AUTOUIC ON
)

target_link_libraries(QtInterface PUBLIC 
Qt6::Widgets Qt6::OpenGL Qt6::OpenGLWidgets 
Core
Surface
SceneObjects
Lighting
Scene
Rendering
Export     
RenderingGL)

target_include_directories(QtInterface PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${Qt6Widgets_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/external/glfw/include
)