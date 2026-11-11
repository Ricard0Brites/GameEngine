#include "RenderComponent.h"
#include "Logger.h"
#include <string>

RenderComponent::RenderComponent(ERenderMode RenderMode)
{
	if (RenderMode == ERenderMode::None)
		Logger::Log(Logger::ELogCategories::Error, std::string(__func__) + " ERenderMode cannot be 'None'", "RenderComponent");

	Logger::Log(Logger::ELogCategories::Verbose, "HELLO FROM RENDER COMPONENT", "RenderComponent");

}

void RenderComponent::BeginPlay()
{
}

void RenderComponent::Tick(float DeltaSeconds)
{
}
