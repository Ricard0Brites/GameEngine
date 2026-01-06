#include "PhysicsSystem.h"
#include <iostream>


PhysicsSystem::PhysicsSystem()
{
    Init
    (
        [&]()
        {
            //Call start Function Here
            std::cout << "Test Output: Physics System" << std::endl;
        }
    );
}

void PhysicsSystem::Init(const std::function<void()>& Func)
{
    ThreadedTask::Init(Func);
}

void PhysicsSystem::End()
{
    ThreadedTask::End();


}
