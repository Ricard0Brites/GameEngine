#pragma once
#include <vector>
#include <memory>       // For std::weak_ptr, std::shared_ptr, std::unique_ptr
#include <algorithm>    // For std::remove_if

// A delegate system that safely binds to methods of objects managed by std::shared_ptr.
template<typename... Args>
class Delegate
{
    // Base class for a single delegate binding. Hides the object's type.
    struct BaseEntry
    {
        virtual ~BaseEntry() = default;
        virtual void Execute(Args... args) = 0;
        virtual bool IsExpired() const = 0;
    };

    // A delegate entry for a specific object type and method.
    template<typename T>
    struct Entry : BaseEntry
    {
        std::weak_ptr<T> Object;
        void (T::* Method)(Args...);

        Entry(std::shared_ptr<T> obj, void (T::* method)(Args...)) 
            : Object(obj), Method(method) {}

        void Execute(Args... args) override
        {
            // Lock the weak_ptr to get a shared_ptr. If the object is still alive...
            if (auto shared_obj = Object.lock())
            {
                // ...call the method on the valid object.
                (shared_obj.get()->*Method)(args...);
            }
        }

        bool IsExpired() const override
        {
            return Object.expired();
        }
    };

public:
    Delegate() = default;
    ~Delegate() = default; // std::unique_ptr in vector handles all cleanup.

    // Delegates are non-copyable and non-movable to ensure clear ownership.
    Delegate(const Delegate&) = delete;
    Delegate& operator=(const Delegate&) = delete;
    Delegate(Delegate&&) = delete;
    Delegate& operator=(Delegate&&) = delete;

    // Binds a method from an object to this delegate.
    // The object must be managed by a std::shared_ptr.
    template<typename T>
    void Bind(std::shared_ptr<T> obj, void (T::* method)(Args...))
    {
        Entries.push_back(std::make_unique<Entry<T>>(obj, method));
    }

    void Execute(Args... args)
    {
        // First, efficiently remove any entries where the object has been destroyed.
        Entries.erase(std::remove_if(Entries.begin(), Entries.end(),
            [](const std::unique_ptr<BaseEntry>& entry) {
                return entry->IsExpired();
            }), Entries.end());

        // Now, execute the remaining valid entries.
        for (const auto& e : Entries)
        {
            e->Execute(args...);
        }
    }

private:
    std::vector<std::unique_ptr<BaseEntry>> Entries;
};