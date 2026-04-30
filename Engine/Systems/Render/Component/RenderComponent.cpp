#include "RenderComponent.h"
#include "Systems/Render/RenderSystem.h"
#include "Logger.h"


const std::string RenderComponent::LogCategory = "RenderComponent";

void RenderComponent::CreateCommandList()
{
    if (!RenderSystem::DX12Data.GetDevice().Get())
    {
        Logger::Log(Logger::ELogCategories::Critical, "RenderComponent::CreateCommandsList: RenderSystem::DX12Data.Device.Get is nullptr", LogCategory);
        __debugbreak();
        return;
    }
    /*
    RenderSystem::DX12Data.GetDevice()->CreateCommandList(0, 
        D3D12_COMMAND_LIST_TYPE_DIRECT, 
        DX12Data.CommandAllocator.Get(),
        PSO.Get(),
        IID_PPV_ARGS(&DX12Data.CommandList));
    DX12Data.CommandList->Close();
    */
}