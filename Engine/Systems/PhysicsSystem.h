#pragma once
#include "Core/ThreadedTask.hpp"

class ENGINE_API PhysicsSystem : public ThreadedTask
{
public:
    PhysicsSystem();

    void Init(const std::function<void()>& Func) override;
    void End() override;


};
