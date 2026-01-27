#include "FlagManager.h"

FlagManager::FlagManager()
{

}

FlagManager::~FlagManager() = default;

auto FlagManager::Update() const -> void
{    
    for (const std::unique_ptr<Flag>& flag : this->flags)
	{   
        if (flag->GetLastTickPlayers().empty())
        {
            break;
        }
        
        flag->Update();
	}

    Flag::GetPlayers();
}

template<typename F>
auto FlagManager::RegisterFlag() -> void
{
	this->flags.emplace_back(std::make_unique<F>());
}