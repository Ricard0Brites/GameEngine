#include "RenderSystem.h"
#include <iostream>


RenderSystem::RenderSystem()
{
    Init
    (
        [&]() 
        {

        }
    );
}

void RenderSystem::Init(const std::function<void()>& Func)
{
    ThreadedTask::Init(Func);

}

void RenderSystem::End()
{
    ThreadedTask::End();
}
