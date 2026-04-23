#pragma once
#include "Core/Core.h"
#include <string>

class  IRenderableObject
{
public:
	IRenderableObject(const char* PathToShader);
	
	inline bool Valid() { return isShaderValid; }


private:
	std::string Shader = "";

	bool isShaderValid = false;

};