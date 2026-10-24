#pragma once
#include <vector>

class ComponentBase;
// TODO - Finish implementing
class SystemBase
{
public:
	SystemBase();



private:
	/* Component Instance */
	static std::vector<ComponentBase> ComponentsCache;
};