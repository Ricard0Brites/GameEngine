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

	template<typename T>
	void Remove(T* obj, void (T::* method)(Args...))
	{
		Entries.erase(std::remove_if(Entries.begin(),Entries.end(),[&](BaseEntry* base)
			{
                Entry<T> *e = dynamic_cast<Entry<T>*>(base);
				if (!e) return false;

				if (e->Object == obj && e->Method == method)
				{
					delete e;
					return true;
				}
				return false;
			}),
			Entries.end());
	}
    
    void Clear()
    {
        Entries.clear();
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