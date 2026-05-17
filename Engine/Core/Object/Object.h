#include "Core/Core.h"
#include <string>
#include <type_traits>

class Object
{
	
public:
	Object(Object* InOwner);
	Object(Object* InOwner, const char* InDisplayName);
	virtual void BeginPlay() = 0;
	virtual void Tick(float DeltaSeconds) = 0;
	uint64_t GetGID() { return ObjectData.GetGID(); }


protected:
	struct FObjectData
	{
	public:
		FObjectData();

		#pragma region Getters

		const std::string& GetDisplayName();
		Object* GetOwner();
		bool GetIsPendingKill();
		uint64_t GetGID();

		// NOTE - Add as needed

		#pragma endregion

		#pragma region Setters

		void SetDisplayName(const char* InName);
		void SetOwner(Object* InOwner);
		void SetPendingKill();
		void OnInitializedStarted();
		void OnInitializedFinished();

		#pragma endregion

		template<typename T> requires std::is_base_of_v<Object, T>
		std::shared_ptr<T> CreateChildOfClass();

	private:
		// Name of the object
		std::string DisplayName = "";

		// List of children (We own them via unique ptr)
		std::vector<std::unique_ptr<Object>> Children = {};

		//nullptr id means this object is independent
		Object* Owner = nullptr;

		// Defines if the object is going to be destroyed or not (TODO - WIP GC)
		ELifeCycleState LifecycleState = ELifeCycleState::Uninitialized;

		uint64_t GlobalID = -1;

		static uint64_t GlobalIDCounter;
	};
	FObjectData ObjectData;
};
