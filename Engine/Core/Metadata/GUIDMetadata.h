#pragma once
#include "Logger.h"

/* 
* Defines a class and an instance GUID
* To access:
* _IDMetadata.ClassID;
* _IDMetadata.InstanceID;
*/
#define GUID_META \
private:\
    struct IDMetadata \
    {\
        IDMetadata() \
        {\
            if (ClassID.Data1 == 0 && ClassID.Data2 == 0 && ClassID.Data3 == 0) \
            {\
                if (((HRESULT)(CoCreateGuid(&ClassID))) < 0) \
                {\
                    Logger::Log(Logger::ELogCategories::Critical, std::string(__FILE__) + " " + std::string(__FUNCTION__) + " failed to assign class GUID", "GUID");\
                }\
            }\
            if (((HRESULT)(CoCreateGuid(&InstanceID))) < 0) \
            {\
                Logger::Log(Logger::ELogCategories::Critical, std::string(__FILE__) + " " + std::string(__FUNCTION__) + " failed to assign Instance GUID", "GUID");\
            }\
        }\
        inline static GUID ClassID{}; \
        GUID InstanceID{};\
    };\
    IDMetadata _IDMetadata; /* Defines Class Global ID and instance Global ID*/\
public:\
    inline const GUID& GetInstanceGUID() const {\
        return _IDMetadata.InstanceID;\
    }\
    static inline const GUID& GetClassGUID() {\
        return IDMetadata::ClassID;\
    }
