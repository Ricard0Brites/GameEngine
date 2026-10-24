#include "RenderComponent.h"


#include "Logger.h" // TODO REMOVE

RenderComponent::RenderComponent() : ComponentBase(EEngineSystem::Render)
{
	Logger::Log(Logger::ELogCategories::Verbose, "RENDER COMPONENT CONSTRUCT", "RenderComponent");
	Logger::Log(Logger::ELogCategories::Verbose, GUIDToString(GetInstanceGUID()), "RenderComponent");
	Logger::Log(Logger::ELogCategories::Verbose, GUIDToString(GetClassGUID()), "RenderComponent");

}
