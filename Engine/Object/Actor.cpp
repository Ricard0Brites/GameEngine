#include "Actor.h"
#include "Core\DataTypes.h"

Actor::Actor(Actor* Parent)
{
	Owner = Parent;	
}

Actor::Actor(Actor* Parent, const char* DisplayName) : Actor(Parent)
{
	#ifdef _DEBUG
		ActorDisplayName = DisplayName;
	#endif // _DEBUG

}

Actor::~Actor()
{
}

void Actor::Tick(float Delta)
{
	if (IsPendingKill) return;
	
}

void Actor::BeginPlay()
{
}
