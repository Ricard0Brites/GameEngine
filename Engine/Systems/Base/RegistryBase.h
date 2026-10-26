#pragma once
#include <memory>
#include <vector>
#include <utility>
#include <mutex>

/*
    Object type must be an interface to allow dynamic casting
*/
template<typename RegistryObjectType>
class RegistryBase
{
public:
    void Register(std::shared_ptr<RegistryObjectType> Object)
    {
        std::lock_guard<std::mutex> Lock(Mutex);
        RegisteredObjects.push_back(Object);
    }

    template<typename PolymorphicClass = RegistryObjectType>
    std::vector<std::shared_ptr<PolymorphicClass>> Get() const
    {
        std::lock_guard<std::mutex> Lock(Mutex);
        std::vector<std::shared_ptr<PolymorphicClass>> Cache = {};
        for (const auto& Wptr : RegisteredObjects)
        {
            if (auto Sptr = Wptr.lock())
            {
                if (auto Casted = std::dynamic_pointer_cast<PolymorphicClass>(Sptr))
                {
                    Cache.push_back(Casted);
                }
            }
        }

        return Cache;
    }

private:
    mutable std::mutex Mutex;
    std::vector<std::weak_ptr<RegistryObjectType>> RegisteredObjects;
};
