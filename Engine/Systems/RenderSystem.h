#pragma once
#include "Core/ThreadedTask.hpp"

class ENGINE_API RenderSystem : public ThreadedTask
{
public:
    RenderSystem();

    void Init(const std::function<void()>& Func) override;
    void End() override;

private:


};
