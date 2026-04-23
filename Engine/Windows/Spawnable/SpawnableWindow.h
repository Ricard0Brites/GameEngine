#pragma once
#include "Core/Core.h" // Memory

#include "Windows/WindowBase.h" // WindowBase

// TODO - Finish implementing SpawnableWindow
class ENGINE_API SpawnableWindow : public WindowBase
{
	#pragma region Constructor & Destructor
public:
	SpawnableWindow();
	~SpawnableWindow();

	#pragma endregion

	#pragma region WindowBase Virtual Functions Implementation
protected:
	/*
	Although this function is virtual avoid overriding it unless you want explicit behavior from Win32 API
	--> Do not forget to call the parent <--
	*/
	virtual void OnMessageReceived(UINT msg) override;

	/*
	* Do not forget to call the parent!
	*/
	virtual void OnDestroy() override;

	#pragma endregion

	#pragma region Helpers

public:
	std::weak_ptr<SpawnableWindow> GetParent();

	#pragma endregion

	#pragma region Events

public:
	Delegate<> OnWindowDestroyedDelegate;

	#pragma endregion

	#pragma region Pointer To Implementation

private:
	struct FSpawnableWindowData
	{
		FSpawnableWindowData()
		{
			ParentWindow.reset();
		}
		~FSpawnableWindowData() = default;
		std::weak_ptr<SpawnableWindow> ParentWindow;
	};

	std::unique_ptr<FSpawnableWindowData> SpawnableWindowData = nullptr;

	#pragma endregion
};