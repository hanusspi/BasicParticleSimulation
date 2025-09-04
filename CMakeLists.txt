cmake_minimum_required(VERSION 3.10)
project(ParticleSimulationSuite LANGUAGES CXX)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add platform-specific settings
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# Global external dependencies setup
set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/external)

# Add GLAD
include_directories(${EXTERNAL_DIR}/glad/include)

# Add GLFW (build once at root level)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)  
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(${EXTERNAL_DIR}/glfw)

# Add GLM
include_directories(${EXTERNAL_DIR}/glm)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Build RenderingEngine as a static library
add_subdirectory(RenderingEngine)

# Build ParticleSimulation using RenderingEngine library
add_subdirectory(ParticleSimulation)

# Set ParticleSimulation as the startup project in Visual Studio
if(MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ParticleSimulation)
endif()

