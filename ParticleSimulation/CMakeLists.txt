cmake_minimum_required(VERSION 3.10)

# ParticleSimulation Application
# This demonstrates how to use the RenderingEngine library in your own project

# Include local headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Source files for the particle simulation application
set(PARTICLE_SIMULATION_SOURCES
    src/main.cpp
    src/application.cpp
    src/particle_simulation.cpp
    src/world_simulation.cpp
    src/particle_system.cpp
    src/physics_engine.cpp
    src/particle.cpp
)

# Create the executable
add_executable(ParticleSimulation ${PARTICLE_SIMULATION_SOURCES})

# Link with the RenderingEngine library
target_link_libraries(ParticleSimulation RenderingEngine)

# Set C++17 standard
set_target_properties(ParticleSimulation PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# Copy shader files to executable directory
add_custom_command(TARGET ParticleSimulation POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/RenderingEngine/shaders
    $<TARGET_FILE_DIR:ParticleSimulation>/shaders
    COMMENT "Copying shaders to build directory"
)

# Platform-specific settings
if(MSVC)
    target_compile_definitions(ParticleSimulation PRIVATE _CRT_SECURE_NO_WARNINGS)
    
    # Set working directory for Visual Studio debugging
    set_target_properties(ParticleSimulation PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/ParticleSimulation/$<CONFIG>"
    )
    
    # Configure as startup project  
    set_property(TARGET ParticleSimulation PROPERTY VS_STARTUP_PROJECT ON)
endif()