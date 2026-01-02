#pragma once

#include <vector>

template<typename... Args>
class Delegate
{
    struct BaseEntry
    {
        virtual void Execute(Args... args) = 0;
        virtual ~BaseEntry() = default;
    };

    template<typename T>
    struct Entry : BaseEntry
    {
        T* Object;
        void (T::* Method)(Args...);

        Entry(T* obj, void (T::* method)(Args...))
            : Object(obj), Method(method) {
        }

        void Execute(Args... args) override
        {
            (Object->*Method)(args...);
        }
    };

public:
    template<typename T>
    void Bind(T* obj, void (T::* method)(Args...))
    {
        Entries.push_back(new Entry<T>(obj, method));
    }

    void Execute(Args... args)
    {
        for (auto* e : Entries)
            e->Execute(args...);
    }

    ~Delegate()
    {
        for (auto* e : Entries)
            delete e;
    }

private:
    std::vector<BaseEntry*> Entries;
};