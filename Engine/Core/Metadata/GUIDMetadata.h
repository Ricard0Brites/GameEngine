#pragma once
#include "Logger.h"

/* 
* Defines a class and an instance GUID
* To access:
* _IDMetadata.ClassID;
* _IDMetadata.InstanceID;
*/
#define GUID_META \
struct IDMetadata\
{\
IDMetadata()\
{\
if (FAILED(CoCreateGuid(&ClassID)))\
{\
Logger::Log(Logger::ELogCategories::Critical, std::string(__FILE__) + " " + std::string(__FUNCTION__) + "failed to assign class GUID", "GUID");\
}\
if (FAILED(CoCreateGuid(&InstanceID)))\
{\
Logger::Log(Logger::ELogCategories::Critical, std::string(__FILE__) + " " + std::string(__func__) + "failed to assign Instance GUID", "GUID");\
}}\
inline static GUID ClassID;\
GUID InstanceID;\
};\
IDMetadata _IDMetadata;