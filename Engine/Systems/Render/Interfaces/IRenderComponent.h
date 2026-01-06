#pragma once
#include "Core/Core.h"

class ENGINE_API IRenderComponent
{
public:
	IRenderComponent() = delete;
	
	IRenderComponent(const char* SpritePath);
	~IRenderComponent();


	struct FSprite;

	virtual const FSprite& GetSprite() = 0;

private:
	FSprite *SpriteRef = nullptr;
};

