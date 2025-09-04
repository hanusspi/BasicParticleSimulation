# Visual Studio CMake Integration Guide

## Quick Start with Visual Studio

### Option 1: Using the Batch Script (Recommended)
1. Run `build_visual_studio.bat` from the project root
2. Open `build/ParticleSimulationSuite.sln` in Visual Studio
3. Right-click `ParticleSimulation` → Set as Startup Project
4. Press F5 to run

### Option 2: Using CMake GUI
1. Open CMake GUI
2. Set source path to project root
3. Set build path to `project_root/build`
4. Configure with "Visual Studio 17 2022" generator
5. Generate
6. Open `build/ParticleSimulationSuite.sln`

### Option 3: Visual Studio CMake Integration
1. File → Open → CMake
2. Select the root `CMakeLists.txt`
3. Wait for CMake configuration
4. Select ParticleSimulation as startup project
5. Build and run

## Common Issues and Solutions

### 1. Shader Files Not Found
**Problem**: Application starts but shows black screen or shader compilation errors.

**Solution**: 
- Shaders are automatically copied to the build directory
- Check that `build/ParticleSimulation/Debug/shaders/` contains:
  - `vertex_shader.glsl`
  - `fragment_shader.glsl`

### 2. Wrong Startup Project
**Problem**: Visual Studio tries to run the wrong project (like `ALL_BUILD`).

**Solution**: 
- Right-click `ParticleSimulation` in Solution Explorer
- Select "Set as Startup Project"
- Or use the dropdown in the toolbar

### 3. Working Directory Issues
**Problem**: Application can't find resources even though they exist.

**Solution**: 
- The CMake configuration sets the working directory automatically
- If issues persist, manually set in Project Properties:
  - Right-click ParticleSimulation → Properties
  - Debugging → Working Directory: `$(OutDir)`

### 4. OpenGL/Graphics Driver Issues
**Problem**: Application crashes or shows OpenGL errors.

**Solution**:
- Update graphics drivers
- Ensure OpenGL 3.3+ support
- Run the application from command line to see error messages

### 5. CMake Cache Issues
**Problem**: Changes to CMakeLists.txt not taking effect.

**Solution**:
- Delete the entire `build` folder
- Re-run CMake configuration
- Or use: Tools → CMake → Delete Cache and Reconfigure

## Project Structure After Build

```
build/
├── ParticleSimulationSuite.sln        # Main Visual Studio solution
├── RenderingEngine/
│   └── RenderingEngine.lib             # Static library
├── ParticleSimulation/
│   └── Debug/
│       ├── ParticleSimulation.exe      # Main executable
│       └── shaders/                    # Copied shader files
└── external/
    └── glfw/                           # GLFW library build
```

## Debug Configuration

The project is configured to:
- Use Debug configuration by default
- Set ParticleSimulation as startup project
- Copy shaders to the correct directory
- Set working directory for debugging

## Build Configurations

- **Debug**: Full debug info, no optimizations
- **Release**: Optimized for performance
- **RelWithDebInfo**: Release with debug info
- **MinSizeRel**: Optimized for size

Switch configurations using the dropdown in Visual Studio toolbar.