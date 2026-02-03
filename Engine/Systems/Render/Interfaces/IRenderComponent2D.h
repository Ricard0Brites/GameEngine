#pragma once
#include "Core/Core.h"

class ENGINE_API IRenderComponent2D
{
public:
	IRenderComponent2D() = delete;
	
	IRenderComponent2D(const char* SpritePath);
	~IRenderComponent2D();


	struct FSprite;

	virtual const FSprite& GetSprite() = 0;

private:
	FSprite *SpriteRef = nullptr;
};

