# RenderingEngine

A reusable C++ rendering engine built with OpenGL that supports multiple geometry types, full transformation matrices, and efficient instanced rendering.

## Features

- **Multiple Geometry Types**: Spheres, cubes, cylinders
- **Full 4x4 Transformation Matrices**: Translation, rotation, scaling, shearing
- **Instanced Rendering**: Efficient rendering of many objects
- **Modern OpenGL**: Core profile 3.3+
- **Abstract Interface**: Easy to extend and integrate

## Core Components

### 1. Renderer Interface

```cpp
class Renderer {
public:
    virtual ~Renderer() = default;
    
    // Core lifecycle
    virtual bool initialize(int width, int height, const char* title) = 0;
    virtual void render(const RenderData& data, const Camera& camera) = 0;
    virtual void shutdown() = 0;
    
    // Window management
    virtual bool shouldClose() const = 0;
    virtual void swapBuffers() = 0;
    virtual void pollEvents() = 0;
    
    // Utilities
    virtual void* getWindow() = 0;
    virtual void getFramebufferSize(int* width, int* height) = 0;
    virtual double getTime() = 0;
    virtual void setVSync(bool enabled) = 0;
};
```

### 2. Data Structures

#### InstanceData
```cpp
struct InstanceData {
    glm::mat4 modelMatrix;  // Full 4x4 transformation matrix
    glm::vec3 color;        // Instance color
    
    // Constructors
    InstanceData(const glm::mat4& matrix, const glm::vec3& col);
    InstanceData(const glm::vec3& position, const glm::vec3& rotation, 
                 const glm::vec3& scale, const glm::vec3& col);
    InstanceData(const glm::vec3& position, const glm::vec3& rotation, 
                 float uniformScale, const glm::vec3& col);
};
```

#### GeometryData
```cpp
struct GeometryData {
    std::string name;                    // Unique identifier
    std::vector<glm::vec3> vertices;     // Vertex positions
    std::vector<glm::vec3> normals;      // Vertex normals
    std::vector<unsigned int> indices;   // Triangle indices
};
```

#### RenderGroup
```cpp
struct RenderGroup {
    GeometryData geometry;               // Base geometry
    std::vector<InstanceData> instances; // Instance transforms/colors
    size_t instanceCount;                // Number of instances
};
```

#### RenderData
```cpp
struct RenderData {
    std::unordered_map<std::string, RenderGroup> renderGroups;
    
    void addRenderGroup(const std::string& name, const GeometryData& geometry);
    void addInstance(const std::string& geometryName, const InstanceData& instance);
    RenderGroup* getRenderGroup(const std::string& name);
};
```

### 3. World Management

#### World Class
```cpp
class World {
public:
    void initialize();
    void getRenderData(RenderData& renderData);
    
    // Add objects
    void addSphere(const glm::vec3& position, float radius, const glm::vec3& color);
    void addCube(const glm::vec3& position, const glm::vec3& scale, 
                 const glm::vec3& rotation, const glm::vec3& color);
    void addContainer(const glm::vec3& size, const glm::vec3& position = glm::vec3(0.0f));
    
    void clear();
};
```

### 4. Camera System

```cpp
class Camera {
public:
    Camera(const glm::vec3& position = glm::vec3(0.0f, 0.0f, 2.0f));
    
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix(float aspectRatio) const;
    
    void processKeyboard(int direction, float deltaTime);
    void processMouseMovement(float xOffset, float yOffset);
    void processMouseScroll(float yOffset);
};
```

## Usage Examples

### Basic Setup

```cpp
#include "opengl_renderer.h"
#include "world.h"

// Create renderer
auto renderer = std::make_unique<OpenGLRenderer>();
renderer->initialize(1200, 900, "My Application");

// Create world and camera
World world;
world.initialize();
Camera camera;

// Add some objects
world.addSphere(glm::vec3(0.0f, 0.0f, 0.0f), 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));
world.addCube(glm::vec3(2.0f, 0.0f, 0.0f), 
              glm::vec3(1.0f, 2.0f, 0.5f),           // scale
              glm::vec3(0.0f, 45.0f * 3.14f/180.0f, 0.0f), // rotation
              glm::vec3(0.0f, 1.0f, 0.0f));          // color

// Render loop
RenderData renderData;
while (!renderer->shouldClose()) {
    world.getRenderData(renderData);
    renderer->render(renderData, camera);
    renderer->swapBuffers();
    renderer->pollEvents();
}

renderer->shutdown();
```

### Advanced Usage - Custom Geometry

```cpp
// Create custom geometry
GeometryData customGeometry("my_geometry");
customGeometry.vertices = { /* vertex data */ };
customGeometry.normals = { /* normal data */ };
customGeometry.indices = { /* index data */ };

// Add to render data
RenderData renderData;
renderData.addRenderGroup("my_geometry", customGeometry);

// Add instances
InstanceData instance1(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
InstanceData instance2(glm::vec3(2.0f, 0.0f, 0.0f), glm::vec3(0.0f), 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));

renderData.addInstance("my_geometry", instance1);
renderData.addInstance("my_geometry", instance2);

renderer->render(renderData, camera);
```

### Using Built-in Geometry Generators

```cpp
#include "geometry.h"

// Create sphere geometry
sphere sphereGen(20);  // 20 subdivisions
GeometryData sphereGeometry = sphereGen.toGeometryData("sphere");

// Create cube geometry
cube cubeGen;
GeometryData cubeGeometry = cubeGen.toGeometryData("cube");

// Create cylinder geometry
cylinder cylinderGen(1.0f, 2.0f, 20, 1);  // radius, height, segments, height segments
GeometryData cylinderGeometry = cylinderGen.toGeometryData("cylinder");
```

## Performance Tips

1. **Group by Geometry Type**: Objects using the same geometry are rendered together
2. **Minimize State Changes**: The renderer automatically groups instances by geometry
3. **Use Instanced Rendering**: Perfect for particles, grass, trees, etc.
4. **Efficient Updates**: Only update instance data that changes each frame

## Integration

To use RenderingEngine in your project:

1. Add as a subdirectory in your CMakeLists.txt:
   ```cmake
   add_subdirectory(path/to/RenderingEngine)
   target_link_libraries(YourProject RenderingEngine)
   ```

2. Include headers:
   ```cpp
   #include "opengl_renderer.h"
   #include "world.h"
   #include "camera.h"
   ```

3. Ensure external dependencies are available:
   - GLAD (OpenGL loader)
   - GLFW (windowing)
   - GLM (math library)

## Dependencies

- **GLAD**: OpenGL function loading
- **GLFW**: Cross-platform windowing and input
- **GLM**: OpenGL Mathematics library
- **C++17**: Modern C++ features

## Architecture Benefits

- **Separation of Concerns**: Rendering logic separated from application logic
- **Extensible**: Easy to add new geometry types or renderers
- **Performance**: Efficient instanced rendering with minimal draw calls
- **Modern**: Uses modern OpenGL and C++ practices