set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_AUTOUIC_SEARCH_PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/Components/ui
    ${CMAKE_CURRENT_SOURCE_DIR}/Widgets/ui
)

set(COMPONENT_WIDGETS
  MainWindow
  TransformWidget
  CameraWidget
  LightWidget
  ObjectWidget
  ViewportSettingsWidget
  SkyboxWidget
  TexturesWidget
  MaterialsWidget
  RenderSettingsWidget
  RenderWindow
  ExportWidget
  CubeDialog
  SphereDialog
  PlaneDialog
)

set(WIDGETS_WITH_UI
  Vec3Editor
  SliderSpinBox
  DoubleSliderSpinBox
  FileSelector
)

set(WIDGETS_NO_UI
  OpenGLWidget
  SceneTreeView
  LineEditDouble
  ColorButton
  SquareSpinBox
  TexturePreview
  TexturesListModel
  MaterialsListModel
  DefaultOptionProxyModel
)

function(get_widget_sources widget_list base_dir with_ui out_var)
  set(sources)
  foreach(widget IN LISTS widget_list)
    if(${with_ui})
      list(APPEND sources ${base_dir}/ui/${widget}.ui)
    endif()
    list(APPEND sources
      ${base_dir}/${widget}.cpp
      ${base_dir}/${widget}.hpp
    )
  endforeach()
  set(${out_var} ${sources} PARENT_SCOPE)
endfunction()

get_widget_sources(
  "${COMPONENT_WIDGETS}"
  ${CMAKE_CURRENT_SOURCE_DIR}/Components
  TRUE
  SOURCES_COMPONENTS
)

get_widget_sources(
  "${WIDGETS_WITH_UI}"
  ${CMAKE_CURRENT_SOURCE_DIR}/Widgets
  TRUE
  SOURCES_WIDGETS_WITH_UI
)

get_widget_sources(
  "${WIDGETS_NO_UI}"
  ${CMAKE_CURRENT_SOURCE_DIR}/Widgets
  FALSE
  SOURCES_WIDGETS_NO_UI
)

set(COMPONENT_SOURCES
  ${SOURCES_COMPONENTS}
  ${SOURCES_WIDGETS_WITH_UI}
  ${SOURCES_WIDGETS_NO_UI}
)

add_library(GUI STATIC
  ${COMPONENT_SOURCES}
  Application.cpp
)

set(CMAKE_PREFIX_PATH ${Qt6Widgets_INCLUDE_DIRS})
find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGL OpenGLWidgets)

target_link_libraries(GUI 
  PUBLIC 
    Qt6::Widgets Qt6::OpenGL Qt6::OpenGLWidgets 
    Core
    Surface
    SceneObjects
    Lighting
    Scene
    Rendering
    Export     
    RenderingGL
)

message(STATUS "Qt6 Widgets include directories: ${Qt6Widgets_INCLUDE_DIRS}")
target_include_directories(GUI PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${Qt6Widgets_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/Components/ui
    ${CMAKE_CURRENT_SOURCE_DIR}/Components
    ${CMAKE_CURRENT_SOURCE_DIR}/Widgets
)
