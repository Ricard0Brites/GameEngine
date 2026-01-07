# DataTypes Analysis: FVector and FTransform

This document provides an analysis of the `FVector` and `FTransform` data types, focusing on the recent implementation of thread-safety features.

## Overview

`FVector` and `FTransform` are fundamental data types in the game engine, used to represent position, rotation, and scale of game objects. In a multi-threaded engine, it is crucial that access to these data types is synchronized to prevent race conditions and ensure data integrity.

## Thread-Safety Implementation

To address the need for thread safety, `std::recursive_mutex` has been integrated into both `FVector` and `FTransform` classes. This ensures that any simultaneous read/write operations from different threads are handled safely.

### FVector

The `FVector` class represents a 3D vector with `X`, `Y`, and `Z` components.

#### Changes Made:

*   A single `std::recursive_mutex` has been added to the `FVector` class.
*   All member functions, including operators, now use `std::lock_guard` to lock the mutex, ensuring that operations on `FVector` instances are atomic.

#### Important Consideration:

The member variables `X`, `Y`, and `Z` of `FVector` remain `public` for ease of use and to maintain backward compatibility. However, this means that **direct member access is not thread-safe**.

**Example of unsafe access:**
```cpp
FVector myVector;
// Unsafe direct access from multiple threads
myVector.X = 10.f; 
```

For thread-safe modifications, always use the provided member functions or operators, or implement external locking mechanisms when direct access is unavoidable.

**Example of safe access:**
```cpp
FVector myVector;
// Safe access using member functions/operators
myVector.Fill(10.f); 
myVector += 5.f;
```

### FTransform

The `FTransform` class represents the transformation of an object in 3D space, including its location, rotation, and scale.

#### Changes Made:

*   Instead of a single mutex, `FTransform` now uses a more granular locking strategy with three separate `std::recursive_mutex` instances: one for location, one for rotation, and one for scale (`_locationMutex`, `_rotationMutex`, `_scaleMutex`).
*   This approach allows different threads to modify the location, rotation, and scale components of a single `FTransform` object simultaneously without blocking each other, improving concurrency.
*   Getters and setters for each component (`[Get/Set]Location`, `[Get/Set]Rotation`, `[Get/Set]Scale`, and their relative counterparts) now lock only the relevant mutex.
*   A thread-safe copy constructor and private assignment operator have been implemented to handle copying of `FTransform` objects safely, using `std::lock` to acquire all mutexes without risking a deadlock.

This fine-grained locking mechanism makes operations on `FTransform` objects both thread-safe and highly performant in multi-threaded scenarios.
