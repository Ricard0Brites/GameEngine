#pragma once
#include "Core/Core.h"
#include <concepts>
#include <string>
#include "Core/DataTypes.h"

template <EEngineSystem SystemType>
class ComponentBase
{
public:
	ComponentBase() = default;

	static constexpr EEngineSystem GetOwnerSystemType() { return SystemType; }

	const EEngineSystem& GetOwningSystem() const { return SystemType; }
	const GUID& GetInstanceGUID() { return _IDMetadata.InstanceID; }
	const GUID& GetClassGUID() { return _IDMetadata.ClassID; }
	std::string GUIDToString(GUID InGUID);

private:
	GUID_META
};

template <EEngineSystem SystemType>
std::string ComponentBase<SystemType>::GUIDToString(GUID InGUID)
{
	std::string Retval = "";

	Retval += std::to_string(InGUID.Data1);
	Retval += "-";
	Retval += std::to_string(InGUID.Data2);
	Retval += "-";
	Retval += std::to_string(InGUID.Data3);
	Retval += "-";

	for (auto entry : InGUID.Data4)
	{
		Retval += std::to_string(entry);
		Retval += "-";
	}
	Retval.pop_back();

	return Retval;
}
