#pragma once
#include <vector>
#include "ComponentBase.h"

// TODO - Finish implementing
template<EEngineSystem SystemType>
class EngineSystemBase
{
public:
	EngineSystemBase();

private:
	/* Component Instance */
	static std::vector<ComponentBase<SystemType>> ComponentsCache;
};

template<EEngineSystem SystemType>
EngineSystemBase<SystemType>::EngineSystemBase()
{
	ComponentsCache.empty();
}

template<EEngineSystem SystemType>
__declspec(selectany) std::vector<ComponentBase<SystemType>> EngineSystemBase<SystemType>::ComponentsCache;
