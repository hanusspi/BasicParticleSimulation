cmake_minimum_required(VERSION 3.10)
project(ParticleSimulation LANGUAGES CXX CUDA)

# Add platform-specific settings
if(MSVC)
    # Use static runtime for MSVC
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    # Disable warnings about unsafe functions
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()


# Add GLAD - manually downloaded by user
# Put GLAD in external/glad/include/glad and external/glad/include/KHR
include_directories(${CMAKE_SOURCE_DIR}/external/glad/include)

# Add GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/glfw)
include_directories(${CMAKE_SOURCE_DIR}/external/glfw/include)

# Add GLM
include_directories(${CMAKE_SOURCE_DIR}/external/glm)

# Include project headers
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/cuda)

# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# Add source files
set(SOURCES
    src/main.cpp
    src/glad.c
    src/shader.cpp
    src/geometry.cpp
)

# Create executable
add_executable(ParticleSimulation ${SOURCES})

# Link libraries
target_link_libraries(ParticleSimulation
    ${OPENGL_LIBRARIES}
    glfw
    ${CMAKE_DL_LIBS}
    ${CUDA_LIBRARIES}
    ${CUDA_CUDA_LIBRARY}
    ${CUDA_CUDART_LIBRARY}
)

# Copy shader files to build directory
file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR})

# On Windows, copy required DLLs to binary directory for easier execution
if(WIN32)
    # This is where we would copy DLLs if needed
endif()
