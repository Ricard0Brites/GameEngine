#pragma once

#include "windows.h"
#include <iostream>
#include "Core/Delegate.h"
#include "Core/EngineStatics.h"

#ifdef EXPORTS_ENGINE
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif