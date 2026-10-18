#include "TestActor.h"

TestActor::TestActor(Object* Owner, const char* DisplayName) : Actor(Owner, DisplayName)
{

}

void TestActor::BeginPlay()
{
	SetLocation(FVector(0, 0, 0));
}
