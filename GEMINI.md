# Gemini Game Engine Analysis

This document provides a summary of the C++ game engine project in this repository.

## Project Overview

This is a small-scale C++ game engine built for Windows. The engine is developed as a reusable DLL (`Engine.dll`) and includes a sample game, `SpaceInvaders`, to demonstrate its usage. The project is set up to be built with Visual Studio.

## Architecture

The engine follows a classic object-oriented design:

*   **Engine Core:** The main `Engine` class initializes the system, creates a window, and runs the main game loop.
*   **Windowing:** A `WindowBase` class abstracts the creation and management of a window using the native Win32 API.
*   **Object Model:**
    *   `Object` is likely the base class for all engine objects.
    *   `Actor` inherits from `Object` and represents a game object that can be placed in the world. It includes a transform (`STransform`) and lifecycle methods like `BeginPlay()` and `Tick()`.
*   **Data Types:** The engine defines its own data types in `Core/DataTypes.h`:
    *   `SVector`: A 3D vector class for math operations.
    *   `STransform`: Represents the location, rotation, and scale of an object.
    *   `InputKeyCodes`: An enum for handling a wide variety of keyboard and gamepad inputs.
*   **Event System:** A `Delegate` system is used for handling events, such as window destruction.

## Projects

The Visual Studio solution (`GameEngine.sln`) contains two main projects:

1.  **`Engine`**: A C++ project that compiles into `Engine.dll`. This is the core, reusable game engine.
2.  **`SpaceInvaders`**: A C++ project that creates `SpaceInvaders.exe`. It depends on and links against `Engine.dll` to run the game.

## Building the Project

To build the solution, you will need **Visual Studio** with the **C++ desktop development workload**.

1.  Open the `GameEngine.sln` file in Visual Studio.
2.  Set the Solution Configuration to `Debug` or `Release`.
3.  Set the Solution Platform to `x64`.
4.  Build the solution (typically by pressing `F7` or `Ctrl+Shift+B`, or from the `Build` menu).

This will compile the `Engine` DLL and the `SpaceInvaders` executable. The output files will be placed in a directory like `x64/Debug/`.

## Running the Game

After a successful build, you can run the game:

1.  The executable will be located at `SpaceInvaders/x64/Debug/SpaceInvaders.exe` (or `Release` if you built in that configuration).
2.  You can run it directly from Windows Explorer or by setting the `SpaceInvaders` project as the startup project in Visual Studio and pressing `F5`.

## Development Conventions

*   **Header Guards:** The project uses `#pragma once`.
*   **API Exporting:** The `ENGINE_API` macro is used to handle `dllexport`/`dllimport` for the engine's public classes.
*   **Naming:** Class names are `PascalCase` (e.g., `Actor`, `WindowBase`).
*   **Comments:** C-style `//` comments are used.
*   **File Structure:** Header files (`.h`) and source files (`.cpp`) are generally kept separate.
