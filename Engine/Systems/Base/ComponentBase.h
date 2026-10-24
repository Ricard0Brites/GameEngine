#pragma once
#include "Core/Core.h"

class ComponentBase
{

public:
	ComponentBase(EEngineSystem InOwningSystem);
	const EEngineSystem& GetOwningSystem() { return OwningSystem; }
	const GUID& GetInstanceGUID() { return _IDMetadata.InstanceID; }
	const GUID& GetClassGUID() { return _IDMetadata.ClassID; }
	std::string GUIDToString(GUID InGUID);
private:
	EEngineSystem OwningSystem;
	GUID_META
};