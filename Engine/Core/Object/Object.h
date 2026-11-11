#include "Core/Core.h"
#include <string>
#include <type_traits>
#include "Systems/Registry/Interfaces.h"
#include "Core/ComponentBase.h"

template <typename T>
struct IsDerivedFromComponentBase
{
	static std::true_type Test(const ComponentBase*);
	static std::false_type Test(...);

	static constexpr bool value = decltype(Test(std::declval<T*>()))::value;
};

template<typename T>
concept DerivedFromComponentBase = IsDerivedFromComponentBase<T>::value;

class Object : public IRegistryObject
{
	
public:
	Object(Object* InOwner);
	Object(Object* InOwner, const char* InDisplayName);
	virtual void BeginPlay() = 0;
	virtual void Tick(float DeltaSeconds) = 0;

	template<DerivedFromComponentBase ComponentClass, typename... Args>
	const GUID& AddComponent(Args&&... args);

protected:
	struct FObjectData
	{
	public:
		FObjectData();

		#pragma region Getters

		const std::string& GetDisplayName();
		Object* GetOwner();
		bool GetIsPendingKill();

		// NOTE - Add as needed

		#pragma endregion

		#pragma region Setters

		void SetDisplayName(const char* InName);
		void SetOwner(Object* InOwner);
		void SetPendingKill();
		void CacheComponent(const GUID& ClassGUID, std::unique_ptr<ComponentBase> Component) { ComponentsCache[ClassGUID].push_back(std::move(Component)); }

		#pragma endregion

		#pragma region Delegate Callbacks

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

		struct GUIDComparator
		{
			bool operator()(const GUID& A, const GUID& B) const
			{
				return memcmp(&A, &B, sizeof(GUID)) < 0;
			}
		};

		/*
		* GUID - The GUID of the component class
		* std::unique_ptr of the component itself
		* 
		*/
		std::map<GUID, std::vector<std::unique_ptr<ComponentBase>>, GUIDComparator> ComponentsCache;
	};
	FObjectData ObjectData;

private:
	GUID_META

};

template<DerivedFromComponentBase ComponentClass, typename... Args>
const GUID& Object::AddComponent(Args&&... args)
{

	Logger::Log(Logger::ELogCategories::Verbose, "Attempting to create component '" + std::string(typeid(ComponentClass).name()) + "';", "Engine");
	/*
	note:
	 - now we're creating components that live inside each object... this is not optimized.
	 In the future this should be changed to work with an ECS for cache locality optimizations
	*/

	//Create Component (Heap)
	std::unique_ptr<ComponentBase> Comp = std::make_unique<ComponentClass>(args...);

	GUID ClassGUID = ComponentClass::GetClassGUID();
	if (IsEqualGUID(ClassGUID, GUID_NULL))
	{
		Logger::Log(Logger::ELogCategories::Error, "Attempt to create component classGUID is invalid. '" + std::string(typeid(ComponentClass).name()) + "::AddComponent' returning.", "Engine");
		return GUID_NULL;
	}

	if (!Comp.get())
	{
		Logger::Log(Logger::ELogCategories::Error, "Attempt to create component '" + std::string(typeid(ComponentClass).name()) + "' failed.", "Engine");
		return GUID_NULL;
	}

	Logger::Log(Logger::ELogCategories::Success, "Created component '" + std::string(typeid(ComponentClass).name()) + "' successfully.", "Engine");

	//Cache Instance GUID
	GUID InstanceGUID = Comp->GetInstanceGUID();

	//Change ownership to the array
	ObjectData.CacheComponent(ClassGUID, std::move(Comp));
	
	//return InstanceGUID;
	return ClassGUID;
}
