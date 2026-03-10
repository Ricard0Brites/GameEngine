# DX12 Networked Game Engine (WIP)

This project is a work-in-progress game engine built using DirectX 12 and C++. The primary goal of this project is to learn the ins and outs of modern graphics programming and engine architecture. The project is being developed with a focus on creating a networked multiplayer experience.

## Current Features

*   **Core Engine:**
    *   **Windowing:** Abstracted window creation and management.
    *   **PImpl Idiom:** The main `Engine` class uses the PImpl idiom to hide implementation details and improve compilation times.
    *   **Threaded Task System:** A simple threaded task system is in place, allowing for asynchronous operations. The `RenderSystem` runs on its own thread.
*   **Rendering (DirectX 12):**
    *   **Device and Swapchain:** Initialization of the D3D12 device, command queue, command list, and swapchain.
    *   **Frame Synchronization:** Fence-based synchronization for managing frame presentation.
    *   **Render Targets:** Creation of render target views for the back buffers.
    *   **Resize Handling:** The swapchain is resized to match when the window resolution changes.
*   **Game Framework:**
    *   **Object Model:** A basic `Object` and `Actor` class hierarchy.
    *   **Data Types:** Thread-safe `FVector`, `FVector2`, and `FTransform` data types for handling 3D transformations.

## In Progress

*   **Graphics Pipeline:** Setting up the graphics pipeline state object (PSO).
*   **Asset Management:** Loading and managing assets such as meshes and textures.
*   **Basic Rendering:** Implementing the necessary logic to render a simple mesh (e.g., a triangle or square... for now 2D only. this can act as a base for UI for example).

## Planned

*   **ECS Architecture:** Transitioning to an Entity-Component-System (ECS) architecture for better performance and scalability.
*   **Input System:** A robust input system for handling keyboard and mouse.
*   **Physics System:** A basic physics system for collision detection and response.
*   **Networking:**
    *   Implementation of a low-level networking layer.
    *   Replication of game state.
*   **Multiplayer Test Project:** A simple LAN-based multiplayer game (Space Invaders) to test the networking and engine features.
