# VoxelEngine

## Requirements

- CMake 4.2.0 or higher
- C++17 compatible compiler (MSVC 19.50+, GCC 7+, Clang 5+)
- Visual Studio 2026 (recommended) or compatible IDE

## Dependencies

Dependencies are automatically downloaded via CMake FetchContent on first build:
- **GLFW 3.4** - Window and input handling
- **GLM 1.0.1** - Mathematics library
- **GLAD** - OpenGL loader (included in repository)

No manual dependency installation required.

## Building

```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

On first run, CMake will automatically download GLFW and GLM from GitHub. Internet connection required for initial setup.