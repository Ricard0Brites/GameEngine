#include "TestActor.h"

TestActor::TestActor(Object* Owner, const char* DisplayName) : Actor(Owner, DisplayName)
{
}

void TestActor::BeginPlay()
{
	SetLocation(FVector(0, 0, 0));
}

void TestActor::Tick(float DeltaSeconds)
{
	Logger::LogTemp("This is a test message frm TestActor::Tick(); Delta Seconds -> " + std::to_string(DeltaSeconds) + ";");
}
