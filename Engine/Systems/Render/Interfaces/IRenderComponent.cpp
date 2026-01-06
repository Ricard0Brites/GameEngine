#include "IRenderComponent.h"
#include <string>
#include "Core/DataTypes.h"

struct IRenderComponent::FSprite
{
	std::string Path = "";

	//Background Removal - Eg: Green Screen 
	bool RemoveBackground = false;
	FColor BackgroundColorSource, BackgroundColorTarget;

};

IRenderComponent::IRenderComponent(const char* SpritePath)
{
	if (SpritePath)
	{
		SpriteRef = new FSprite;
		SpriteRef->Path = SpritePath;
	}
}

IRenderComponent::~IRenderComponent()
{
	if (SpriteRef)
	{
		delete SpriteRef;
		SpriteRef = nullptr;
	}
}

const IRenderComponent::FSprite& IRenderComponent::GetSprite()
{
	return *SpriteRef;
}

