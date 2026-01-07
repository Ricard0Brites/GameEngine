#include "Actor.h"
#include "Core\DataTypes.h"

struct Actor::FActorData
{
	std::string ActorDisplayName = "";
};

Actor::Actor(Actor* Parent)
{
	Owner = Parent;	
	Data = new FActorData;
}

Actor::Actor(Actor* Parent, const char* DisplayName) : Actor(Parent)
{
	if(Data)
		Data->ActorDisplayName = DisplayName;
}

Actor::~Actor()
{
	if (Data)
	{
		delete Data;
		Data = nullptr;
	}
}

void Actor::Tick(float Delta)
{
	if (IsPendingKill) return;
	
}

void Actor::BeginPlay()
{
}
