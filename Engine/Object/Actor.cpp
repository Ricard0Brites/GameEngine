#include "Actor.h"
#include "Core\DataTypes.h"

Actor::Actor(Object* Owner) : 
	Object(Owner)
{

}

Actor::Actor(Object* Owner, const char* DisplayName) : 
	Object(Owner, DisplayName)
{

}

Actor::~Actor()
{
}

void Actor::Tick(float Delta)
{
	if (ObjectData->GetIsPendingKill()) return;
	
}

void Actor::BeginPlay()
{
}
