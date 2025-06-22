# Lumen

**Lumen** is a modern C++20 3D rendering engine built from scratch to showcase high-performance rendering, clean architecture, and real-time + offline graphics capabilities.
It reflects my expertise in computer graphics and professional software engineering, with a modular design, real-time OpenGL viewport, and a custom CPU-based ray tracer.

![Viewport with some objects](Gallery/ViewportWithObjects.png)

## ✨ Features

- **Texturing:** Supports albedo and normal mapping, with configurable wrapping (`REPEAT`, `MIRROR_REPEAT`, `CLAMP_TO_EDGE`, `CLAMP_TO_BORDER`) and filtering (`BILINEAR`, `NEAREST`) modes
- **Modular Architecture:** Clean separation of concerns across different modules
- **Mesh Generation & Loading:** Procedural generation of cubes, spheres, and planes, along with OBJ file loading capabilities
- **Advanced Lighting:** Implements directional, point, and spot lights with customizable parameters
- **Camera System:** Configurable camera with aperture, focus distance, field of view, and transformation controls
- **OpenGL Rendering View:** Real-time viewport with support for lights, shadows, tone mapping, gamma correction, textured materials, and skybox rendering  
- **Qt User Interface:** Fully-featured Qt interface following the MVC architecture to control and inspect all scene elements dynamically  
- **Skybox Integration:** Supports environment mapping using skybox textures
- **Image Exporting:** Renders can be exported as PNG, JPEG, BMP, TGA and HDR images with tone mapping and exposure adjustments
- **Ray Acceleration:** Ray traversal acceleration with a *Bounding Volume Hierarchy* (BVH)
- **Ray Tracing Renderer:** High-quality image rendering using a CPU-based ray tracer, with both single-threaded and multi-threaded execution modes  
- **Comprehensive CI:** Automated formatting, linting and testing via GitHub Actions

## 🧩 Architecture Overview

The project is structured into distinct modules to ensure maintainability and scalability:

- **Core:** Fundamental utilities and data structures
- **Surface:** Manages materials and textures
- **Geometry:** Responsible for mesh creation and loading
- **Lighting:** Defines different light types and their behaviors
- **Scene:** Manages objects, camera, lights, and the skybox
- **Rendering:** Handles the rendering pipeline and settings
- **OpenGL:** Manages the real-time rendering view, shaders, framebuffers, and OpenGL-specific resources  
- **Qt:** Provides the user interface and interaction logic, built with an MVC architecture for clean separation between UI, data, and control flow  
- **Export:** Manages the export of rendered images

This modular design facilitates independent development and testing of each component, promoting clean code practices and ease of maintenance.

## 📚 Documentation

The project is available with a Doxygen documentation. The documentation has been deployed with GitHub Pages and is accessible from this link : [https://lucas-gtr.github.io/Lumen/](https://lucas-gtr.github.io/Lumen/).

It includes detailed class references and module overviews.

## 🖼️ Rendering Pipeline

- **Ray Sampling**: Rendering uses a Monte Carlo approach. For each pixel, several rays are sampled using stratified sampling within a grid, ensuring uniform stochastic coverage across the pixel area. This improves convergence and reduces noise in the output.
- **Flexible Sample Count**: The number of rays per pixel (render samples) is configurable, allowing a trade-off between quality and performance
- **No Indirect Lighting:** Currently, no global illumination or path tracing is implemented.
- **Tone Mapping:** Final color output can be mapped using Reinhard or Exposure techniques, or simply clamped for raw output.
- **Color Accuracy:** All lighting computations are performed in linear space, with gamma correction applied when writing to sRGB.

## 🛠️ Build & Run Instructions

### Prerequisites

- C++20 compatible compiler (e.g., GCC 10+, Clang 11+, MSVC 2019+)
- CMake 3.10 or higher
- OpenGL 3.3 or higher
- Qt 6.x with the following modules:
  - `QtWidgets`
  - `QtOpenGLWidgets`

### Building the Project

```bash
git clone --recurse-submodules https://github.com/lucas-gtr/Lumen.git
cd Lumen
mkdir build && cd build
cmake ..
make
```

### Optional Build Flags
- `ENABLE_COMPILE_WARNINGS`: Enable compiler warnings (default: ON)
- `ENABLE_SANITIZERS`: Enable address and undefined behavior sanitizers (default: ON)
- `ENABLE_COMPILER_OPTIMIZATIONS`: Enable compiler optimizations (default: ON)
- `ENABLE_LTO`: Enable Link Time Optimization (default: OFF)
- `ENABLE_CLANG_FORMAT`: Enable code formatting checks (default: OFF)
- `ENABLE_CLANG_TIDY`: Enable static analysis checks (default: OFF)
- `ENABLE_DOXYGEN`: Generate documentation using Doxygen (default: OFF)
- `ENABLE_TESTS`: Build and run unit tests (default: OFF)

### Running the Application
```bash
make run
```

This command will simply launches the Qt application.  
From the graphical interface, you can then:
- Create various meshes (cube, sphere, plane)
- Load OBJ models
- Add and customize directional, point, and spot lights
- Set up materials with diffuse texture and normal maps
- Configure a camera with aperture, focus distance, and field of view
- Adjust tone mapping, exposure, and post-processing settings in real-time
- Trigger a ray-traced render (single- or multi-threaded) and export the image

## ✅ Continuous Integration
The project employs GitHub Actions for continuous integration, ensuring code quality and reliability through automated workflows:

- Formatting & Linting: Utilizes clang-format and clang-tidy to enforce code style and detect potential issues.
- Testing: Runs unit tests using GoogleTest to validate functionality.

These workflows are triggered on pull requests to the `main` branch, maintaining code integrity and facilitating collaborative development.

### 🧪 Testing & Coverage
Unit tests are located in the tests/ directory and can be executed as follows:

```bash
make run-tests
```

To generate a coverage report:

```bash
make coverage
```

## Commands

These are the available Make targets:

| Command               | Description                                                                 |
|-----------------------|-----------------------------------------------------------------------------|
| `make run`            | Build and run the Lumen application                                         |
| `make build`          | Build the project if configured                                             |
| `make configure`      | Configure the main development build environment                            |
| `make format`         | Run clang-format over the entire codebase                                   |
| `make lint`           | Run static analysis using clang-tidy                                        |
| `make format-and-lint`| Run both clang-format and clang-tidy checks                                 |
| `make configure-tests`| Set up the build environment for unit tests                                 |
| `make run-tests`      | Build and run the test suite using GoogleTest                               |
| `make coverage`       | Generate a coverage report using gcovr (HTML and JSON output)               |
| `make update-readme`  | Inject the latest coverage report into the README.md                        |
| `make generate-doc`   | Generate Doxygen documentation into `docs/html`                             |
| `make clean`          | Remove all build directories and clean temporary artifacts

## 🚧 Future Improvements

Planned enhancements for future development:

- **Auto-generated modular UI**: A powerful and flexible interface system driven by JSON schemas, enabling automatic generation of control panels (e.g., camera, lighting, materials) with customizable widgets, value ranges, and live bindings
- **GPU acceleration with CUDA** for real-time performance
- Integration of **full rendering equation** and support for **physically-based rendering** (PBR)

## 🙋 About me

I'm a software engineer specializing in 3D development and modern C++ practices. Lumen is a testament to my passion for graphics programming and clean software architecture

📧 Contact me at: l_gottar@orange.fr

🔗 Connect with me on [LinkedIn](https://www.linkedin.com/in/lucas-gottar-856070234/)

## 📸 Gallery

**Default scene when starting the application**

![Lumen default interface](Gallery/LumenInterface.png)

**Export window to visualize and export the rendering**

![Lumen default interface](Gallery/ExportInterface.png)

*Image 1920x1080 - 34581 triangles - 2 lights - 4 samples per pixel* | **Render time single-threaded**: 2min 11s | **Render time multi-threaded (28 threads)**: 10s

![OBJ example](Gallery/RenderTest.png)



