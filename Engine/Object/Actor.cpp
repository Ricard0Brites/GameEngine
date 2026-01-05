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

void Actor::Tick(float DeltaSeconds)
{
	if (IsPendingKill) return;
	if (Owner)
	{
		if (Actor* OwnerActor = dynamic_cast<Actor*>(Owner))
			SetLocation(OwnerActor->GetLocation() + GetRelativeLocation());
	}
}

void Actor::BeginPlay()
{
}
