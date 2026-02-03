#include "IRenderComponent2D.h"
#include <string>
#include "Core/DataTypes.h"

struct IRenderComponent2D::FSprite
{
	std::string Path = "";

	//Background Removal - Eg: Green Screen 
	bool RemoveBackground = false;
	FColor BackgroundColorSource, BackgroundColorTarget;

};

IRenderComponent2D::IRenderComponent2D(const char* SpritePath)
{
	if (SpritePath)
	{
		SpriteRef = new FSprite;
		SpriteRef->Path = SpritePath;
	}
}

IRenderComponent2D::~IRenderComponent2D()
{
	if (SpriteRef)
	{
		delete SpriteRef;
		SpriteRef = nullptr;
	}
}

const IRenderComponent2D::FSprite& IRenderComponent2D::GetSprite()
{
	return *SpriteRef;
}

