#include "SpawnableWindow.h"
#include <memory> // shared_ptr
#include "Engine.h" // Do not change to .h (will cause a circular dependency)

SpawnableWindow::SpawnableWindow() :
	WindowBase(L"Default Window Title"),
	SpawnableWindowData(std::make_unique<FSpawnableWindowData>())
{
}

SpawnableWindow::~SpawnableWindow()
{

}

void SpawnableWindow::OnMessageReceived(UINT msg)
{
	// TODO - Input Event Handling
}

void SpawnableWindow::OnDestroy()
{
	OnWindowDestroyedDelegate.Execute();
}

std::weak_ptr<SpawnableWindow> SpawnableWindow::GetParent()
{
	return SpawnableWindowData->ParentWindow;
}

