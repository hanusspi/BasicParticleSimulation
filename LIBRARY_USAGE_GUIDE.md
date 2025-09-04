# How to Use RenderingEngine as a Library

This guide demonstrates how the **RenderingEngine** is now a true reusable library that you can integrate into any OpenGL project.

## 🏗️ Build System Architecture

### Current Structure (True Library Separation):
```
ParticleSimulationSuite/
├── RenderingEngine/              # 📚 STATIC LIBRARY (.lib)
│   ├── CMakeLists.txt           # Builds: RenderingEngine.lib
│   └── ...
├── ParticleSimulation/           # 🎮 APPLICATION (.exe)
│   ├── CMakeLists.txt           # Links: RenderingEngine.lib
│   └── ...
└── CMakeLists.txt               # Root: Coordinates both
```

### Build Output:
- **`RenderingEngine.lib`** - Static library file
- **`ParticleSimulation.exe`** - Application linking against library
- **Proper dependency management** - App depends on library

## 🎯 How It Works Now

### 1. RenderingEngine is Built as Library
```cmake
# RenderingEngine/CMakeLists.txt
add_library(RenderingEngine STATIC ${RENDERING_ENGINE_SOURCES})

target_include_directories(RenderingEngine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/glad/include
    ${CMAKE_SOURCE_DIR}/external/glfw/include  
    ${CMAKE_SOURCE_DIR}/external/glm
)

target_link_libraries(RenderingEngine 
    ${OPENGL_LIBRARIES}
    glfw
    ${CMAKE_DL_LIBS}
)
```

### 2. ParticleSimulation Links Against Library
```cmake
# ParticleSimulation/CMakeLists.txt
add_executable(ParticleSimulation ${PARTICLE_SIMULATION_SOURCES})

# This is the key line - linking against the library
target_link_libraries(ParticleSimulation RenderingEngine)
```

## 🚀 Using RenderingEngine in Your Own Project

### Step 1: Copy RenderingEngine to Your Project
```bash
# In your project directory
git submodule add <renderingengine-repo> external/RenderingEngine
# OR simply copy the RenderingEngine folder
```

### Step 2: Your Project's CMakeLists.txt
```cmake
cmake_minimum_required(VERSION 3.10)
project(MyAwesomeProject LANGUAGES CXX)

# Set up external dependencies (GLFW, GLAD, GLM)
# ... (copy from our root CMakeLists.txt)

# Add RenderingEngine library
add_subdirectory(external/RenderingEngine)

# Your application
add_executable(MyApp
    src/main.cpp
    src/my_simulation.cpp
    # ... your source files
)

# Link with RenderingEngine
target_link_libraries(MyApp RenderingEngine)

# Copy shaders
add_custom_command(TARGET MyApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/external/RenderingEngine/shaders
    $<TARGET_FILE_DIR:MyApp>/shaders
)
```

### Step 3: Your Application Code
```cpp
// main.cpp - Your complete application using RenderingEngine
#include "opengl_renderer.h"
#include "world.h"
#include "camera.h"

int main() {
    // 1. Initialize renderer (from library)
    auto renderer = std::make_unique<OpenGLRenderer>();
    renderer->initialize(1200, 900, "My Project");
    
    // 2. Create world and camera (from library) 
    World world;
    world.initialize();
    Camera camera;
    
    // 3. Add your objects using library interface
    world.addSphere(glm::vec3(0, 0, 0), 0.5f, glm::vec3(1, 0, 0));
    world.addCube(glm::vec3(2, 0, 0), 
                  glm::vec3(1, 1, 1),           // scale
                  glm::vec3(0, 0, 0),           // rotation  
                  glm::vec3(0, 1, 0));          // color
    
    // 4. Standard render loop
    RenderData renderData;
    while (!renderer->shouldClose()) {
        world.getRenderData(renderData);
        renderer->render(renderData, camera);
        renderer->swapBuffers();
        renderer->pollEvents();
    }
    
    renderer->shutdown();
    return 0;
}
```

## 🔍 Verification of Library Separation

### Build Output Demonstrates True Separation:
```bash
$ cmake --build .
...
Building RenderingEngine.lib      # ✅ Library built first
Building ParticleSimulation.exe   # ✅ App links against library  
Copying shaders...               # ✅ Assets managed properly
```

### Key Differences from Before:
| Before (Monolithic) | After (Library) |
|---------------------|-----------------|
| All sources compiled into one exe | Library compiled separately |
| No reusability | Can be used in any project |
| Tight coupling | Clean interface separation |
| Single build target | Multiple targets with dependencies |

## 📦 What You Get

### RenderingEngine Library Provides:
- **`Renderer` interface** - Abstract rendering capability
- **`OpenGLRenderer`** - Concrete OpenGL implementation
- **`World` class** - High-level scene management  
- **`Camera` system** - View/projection management
- **Multiple geometry types** - Spheres, cubes, cylinders
- **Full transformation matrices** - Translation, rotation, scale, shearing
- **Instanced rendering** - Efficient for many objects

### Your Application Only Needs:
- **Include RenderingEngine headers** - Clean API
- **Link against RenderingEngine.lib** - Single dependency
- **Implement your logic** - Physics, AI, game rules, etc.
- **Use library interface** - `world.addSphere()`, `renderer->render()`

## ✅ Benefits Achieved

1. **🔄 True Reusability** - Copy RenderingEngine to any project
2. **🏗️ Clean Architecture** - Library vs. application separation  
3. **📚 Library Interface** - Professional API design
4. **🎯 Single Responsibility** - Rendering engine does rendering only
5. **🚀 Performance** - Static library, optimized builds
6. **📖 Documentation** - Complete API reference and examples

The **RenderingEngine** is now a production-ready library that demonstrates proper C++ library design with CMake integration!