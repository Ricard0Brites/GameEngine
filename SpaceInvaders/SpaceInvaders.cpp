//Modules
#include "Engine.h"

int main()
{
	Engine EngineInstance(L"Space Invaders"); // Instance on the stack
	
	// No need to check for validity or delete, destructor will be called automatically
	EngineInstance.Launch(); // Game Loop

	// Any code here is only gonna be run at destroy
}