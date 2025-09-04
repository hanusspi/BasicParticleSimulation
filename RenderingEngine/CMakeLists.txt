cmake_minimum_required(VERSION 3.10)

# RenderingEngine Library
# This creates a reusable static library for OpenGL rendering

# External dependencies are expected to be handled by parent project
# or can be set up here if building standalone

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Source files for the RenderingEngine library
set(RENDERING_ENGINE_SOURCES
    src/core.cpp
    src/opengl_renderer.cpp  
    src/shader.cpp
    src/camera.cpp
    src/geometry.cpp
    src/world.cpp
    src/input_handler.cpp
    ${CMAKE_SOURCE_DIR}/external/glad/src/glad.c
)

# Create the static library
add_library(RenderingEngine STATIC ${RENDERING_ENGINE_SOURCES})

# Set target properties
set_target_properties(RenderingEngine PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# Link with required libraries
target_link_libraries(RenderingEngine 
    ${OPENGL_LIBRARIES}
    glfw
    ${CMAKE_DL_LIBS}
)

# Export include directories for consumers of this library
target_include_directories(RenderingEngine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/glad/include
    ${CMAKE_SOURCE_DIR}/external/glfw/include  
    ${CMAKE_SOURCE_DIR}/external/glm
)

# Platform-specific settings
if(MSVC)
    target_compile_definitions(RenderingEngine PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

# Export the library for external use
set(RENDERINGENGINE_INCLUDE_DIRS 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/glad/include
    ${CMAKE_SOURCE_DIR}/external/glfw/include
    ${CMAKE_SOURCE_DIR}/external/glm
    PARENT_SCOPE
)