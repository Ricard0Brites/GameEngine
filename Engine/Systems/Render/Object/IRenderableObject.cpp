#include "IRenderableObject.h"
#include "Logger.h"
#include <fstream>
#include <filesystem>


IRenderableObject::IRenderableObject(const char* PathToShader)
{
	using namespace std;

	filesystem::path p = filesystem::absolute(PathToShader);
	
	if (!filesystem::exists(p))
	{
		Logger::Log(Logger::ELogCategories::Error, "Shader file not found!", "IRenderableObject");
		return;
	}

}

