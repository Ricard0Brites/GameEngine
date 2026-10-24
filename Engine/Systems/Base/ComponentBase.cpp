#include "ComponentBase.h"
#include <string>

ComponentBase::ComponentBase(EEngineSystem InOwningSystem) : OwningSystem(InOwningSystem)
{

}

std::string ComponentBase::GUIDToString(GUID InGUID)
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

