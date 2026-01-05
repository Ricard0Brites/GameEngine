# Project Analysis Report

This document outlines potential issues and areas for improvement in the C++ game engine project. The analysis covers general C++ best practices, memory management, object lifetime, and class design.

## 1. Critical Issues

These issues are likely to cause crashes, memory leaks, or undefined behavior.

### 1.1. Missing Virtual Destructor in `Object` Base Class

**File:** `Engine/Object/Object.h`

The `Object` class is an abstract base class with pure virtual functions. It is intended to be inherited by other classes like `Actor`. However, it lacks a virtual destructor.

```cpp
// Engine/Object/Object.h
class Object
{
public:
	virtual void BeginPlay() = 0;
	virtual void Tick(float DeltaSeconds) = 0;
    // Missing: virtual ~Object() = default;
};
```

**Problem:** When a derived class instance (e.g., an `Actor`) is deleted through a base class pointer (`Object*`), and the base class destructor is not virtual, only the base class destructor is called. This leads to the derived class's resources not being cleaned up, causing memory leaks and undefined behavior.

**Solution:** Add a virtual destructor to the `Object` class.

```cpp
// In Engine/Object/Object.h
virtual ~Object() = default;
```

### 1.2. `Delegate` Class: Copy Safety and Memory Leaks

**File:** `Engine/Core/Delegate.h`

The `Delegate` class manually manages memory for its `Entry` objects but does not follow the Rule of Three/Five.

**Problem:**
1.  **Double-Free on Copy:** If a `Delegate` object is copied, the vector of `BaseEntry*` pointers is shallow-copied. Both the original and the copy will point to the same allocated `Entry` objects. When one `Delegate` is destroyed, it deletes the `Entry` objects. When the second `Delegate` is destroyed, it attempts to delete the same pointers again, causing a double-free and a crash.
2.  **Dangling Pointers:** The `Delegate` stores raw pointers to objects (`T* Object`). If an object that has bound a function to a delegate is destroyed, the delegate will be left with a dangling pointer. Calling `Execute()` will then result in a crash.

**Solution:**
*   **Disable Copying:** The simplest and safest solution is to make the `Delegate` class non-copyable and non-movable.

    ```cpp
    // In Engine/Core/Delegate.h
    public:
        Delegate() = default;
        ~Delegate()
        {
            for (auto* e : Entries)
                delete e;
        }

        // Disable copy and move
        Delegate(const Delegate&) = delete;
        Delegate& operator=(const Delegate&) = delete;
        Delegate(Delegate&&) = delete;
        Delegate& operator=(Delegate&&) = delete;

        // ... rest of the class
    ```
*   **Handle Dangling Pointers:** This is a harder problem. A common solution is a weak-pointer system, but given the constraints on using `std::shared_ptr`, an alternative is to require explicit unbinding in the destructor of any class that binds to a delegate. This is a design pattern that must be enforced by convention.

### 1.3. `ThreadedTask` Class: Copy Safety and Thread Management

**File:** `Engine/Core/ThreadedTask.hpp`

Similar to `Delegate`, `ThreadedTask` manages a raw pointer (`Pimpl`) but does not follow the Rule of Three/Five, leading to a double-free on copy.

**Problem:**
1.  **Double-Free on Copy:** Copying a `ThreadedTask` will lead to a double-free of the `Pimpl` and `Pimpl->Thread` members.
2.  **Unsafe Destructor:** The destructor deletes the `std::thread*` without ensuring the thread has been joined. This can lead to resource leaks or abrupt thread termination.

**Solution:**
*   **Make `ThreadedTask` Non-Copyable:** Like the `Delegate`, it should be made non-copyable.
*   **Use `std::unique_ptr` for PImpl:** The raw `Pimpl` pointer is a good candidate for `std::unique_ptr` as per your instructions. This simplifies memory management significantly.

    ```cpp
    // In Engine/Core/ThreadedTask.hpp
    #include <memory> // Add this

    class ENGINE_API ThreadedTask
    {
        // ...
    private:
        struct SData
        {
            std::function<void()> StartFunction = nullptr;
            std::unique_ptr<std::thread> Thread = nullptr;
        };
        std::unique_ptr<SData> Pimpl;
    };
    ```
    With `std::unique_ptr`, the custom destructor becomes much simpler and safer, as the smart pointers will handle deletion. You still need to ensure `Join()` is called properly.

## 2. Design and API Issues

### 2.1. `Actor` Inherits from `STransform`

**File:** `Engine/Object/Actor.h`

`Actor` inherits from `STransform`, which represents data for location, rotation, and scale. This is a misuse of public inheritance. Inheritance should model an "is-a" relationship, but an `Actor` is not a transform; it *has* a transform.

**Problem:** This leads to an unintuitive and inflexible design. For example, it pollutes the `Actor`'s public interface with `STransform`'s methods.

**Solution:** Use composition instead of inheritance. `Actor` should contain an `STransform` member variable.

```cpp
// In Engine/Object/Actor.h
#include "Object.h"
#include "Core/DataTypes.h"

class Actor : public Object
{
public:
    // ... constructors, etc.

    // Expose transform through getter
    STransform& GetTransform() { return Transform; }
    const STransform& GetTransform() const { return Transform; }

private:
    STransform Transform;
    // ... other members
};
```

### 2.2. `Actor` Class Not Exported

**File:** `Engine/Object/Actor.h`

The `Actor` class is not decorated with `ENGINE_API`.

**Problem:** If `Actor` is defined in the `Engine.dll` but not exported, the `SpaceInvaders.exe` executable will not be able to link against it, causing linker errors.

**Solution:** Add `ENGINE_API` to the class definition.

```cpp
// In Engine/Object/Actor.h
class ENGINE_API Actor : public Object // ...
```

### 2.3. Inefficient Operators in `SVector`

**File:** `Engine/Core/DataTypes.h`

The operator overloads in `SVector` take their arguments by value and some return by value unnecessarily.

**Problem:** This causes unnecessary copies of `SVector` objects during arithmetic operations, which can be a performance issue in a game engine.

**Solution:**
*   Take vector arguments by `const` reference (`const SVector&`).
*   Make non-modifying operators `const`.
*   Return by reference (`SVector&`) for assignment operators (`+=`, `*=`, etc.).

```cpp
// Example in Engine/Core/DataTypes.h
// Before
SVector operator+=(const SVector a){ X+=a.X, Y+=a.Y, Z+=a.Z; return *this; };
SVector operator+(const SVector a) { SVector temp; temp.X = X + a.X, temp.Y = Y + a.Y, temp.Z = Z + a.Z; return temp;};

// After
SVector& operator+=(const SVector& a){ X+=a.X; Y+=a.Y; Z+=a.Z; return *this; }
SVector operator+(const SVector& a) const { SVector temp; temp.X = X + a.X; temp.Y = Y + a.Y; temp.Z = Z + a.Z; return temp; }
```

## 3. General Best Practices

### 3.1. `using namespace std;` in Header

**File:** `Engine/Core/DataTypes.h`

The header `DataTypes.h` contains `using namespace std;`.

**Problem:** This pollutes the global namespace for any file that includes `DataTypes.h`, potentially leading to name collisions and hard-to-debug errors.

**Solution:** Remove `using namespace std;` from the header. In the `.h` file, qualify names with `std::` (e.g., `std::string`). A `using namespace std;` statement can be safely used within the corresponding `.cpp` file if desired.
