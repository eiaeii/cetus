#include "origin_entity.h"
#include "origin_component.h"

using namespace terra;

OriginEntity* OriginEntity::default_instance_ = nullptr;

OriginEntity* OriginEntity::Get()
{
	if (!default_instance_)
	{
		default_instance_ = new OriginEntity(FGuid::NewGuid());
	}
	return default_instance_;
}

void OriginEntity::Destroy()
{
	delete default_instance_;
}


OriginEntity::OriginEntity(const FGuid& unique_id) : guid_(unique_id) , running_(true) {}

OriginEntity::~OriginEntity()
{
	DestroyAllComponent();
}

bool OriginEntity::Awake()
{
	for (auto&& kv : components_)
	{
		if (!(kv.second)->Awake())
		{
			return false;
		}
	}
	return true;
}

bool OriginEntity::Init()
{
	for (auto&& kv : components_)
	{
		if (!(kv.second)->Init())
		{
			return false;
		}
	}
	return true;
}

void OriginEntity::PreUpdate()
{
	for (auto&& kv : components_)
	{
		(kv.second)->PreUpdate();
	}
}

void OriginEntity::Update()
{
	for (auto&& kv : components_)
	{
		(kv.second)->Update();
	}
}

void OriginEntity::PostUpdate()
{
	for (auto&& kv : components_)
	{
		(kv.second)->PostUpdate();
	}
}

void OriginEntity::Exit()
{
	for (auto&& kv : components_)
	{
		(kv.second)->Exit();
	}
}

OriginEntity& OriginEntity::AddComponent(const int idx, std::unique_ptr<OriginComponent> component)
{
	Expects(!HasComponent(idx));
	components_[idx] = std::move(component);
	return *this;
}

OriginEntity& OriginEntity::RemoveComponent(const int idx)
{
	Expects(HasComponent(idx));
	ReplaceWith(idx, nullptr);
	return *this;
}

OriginEntity& OriginEntity::ReplaceComponent(const int idx, std::unique_ptr<OriginComponent> component)
{
	if (HasComponent(idx))
	{
		ReplaceWith(idx, std::move(component));
	}
	else if (component != nullptr)
	{
		AddComponent(idx, std::move(component));
	}

	return *this;
}

OriginComponent* OriginEntity::GetComponent(const int idx) const
{
	if (!HasComponent(idx))
	{
		return nullptr;
	}

	return (components_.at(idx)).get();
}

bool OriginEntity::HasComponent(const int idx) const
{
	return (components_.find(idx) != components_.end());
}


void OriginEntity::ReplaceWith(const int idx, std::unique_ptr<OriginComponent> replacement)
{
	OriginComponent* prev_component = GetComponent(idx);
	if (prev_component == replacement.get())
	{
	}
	else
	{
		if (replacement == nullptr)
		{
			components_.erase(idx);
		}
		else
		{
			components_[idx] = std::move(replacement);
		}
	}
}

void OriginEntity::DestroyAllComponent()
{
	components_.clear();
}

