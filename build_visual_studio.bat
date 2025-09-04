@echo off
echo Building ParticleSimulation with Visual Studio...

REM Clean build directory
if exist build (
    echo Cleaning build directory...
    rmdir /s /q build
)

REM Create build directory
mkdir build
cd build

REM Configure with Visual Studio generator
echo Configuring with CMake for Visual Studio...
cmake -G "Visual Studio 17 2022" -A x64 ..

REM Check if configuration was successful
if %ERRORLEVEL% NEQ 0 (
    echo CMake configuration failed!
    pause
    exit /b 1
)

REM Build the project
echo Building project...
cmake --build . --config Debug

REM Check if build was successful
if %ERRORLEVEL% NEQ 0 (
    echo Build failed!
    pause
    exit /b 1
)

echo Build completed successfully!
echo You can now open ParticleSimulationSuite.sln in Visual Studio
echo Or run ParticleSimulation.exe from build\ParticleSimulation\Debug\

pause