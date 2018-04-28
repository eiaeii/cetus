#include "entity.h"
#include "entity/component.h"

using namespace terra;

Entity::Entity()
{

}

Entity::~Entity()
{

}

Entity& Entity::AddComponent(const int idx, std::unique_ptr<IComponent> component)
{
	Expects(!HasComponent(idx));
	component->SetOwner(this);
	components_[idx] = std::move(component);
	return *this;
}

Entity& Entity::RemoveComponent(const int idx)
{
	Expects(HasComponent(idx));
	ReplaceWith(idx, nullptr);
	return *this;
}

Entity& Entity::ReplaceComponent(const int idx, std::unique_ptr<IComponent> component)
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

IComponent* Entity::GetComponent(const int idx) const
{
	if (!HasComponent(idx))
	{
		return nullptr;
	}

	return (components_.at(idx)).get();
}

bool Entity::HasComponent(const int idx) const
{
	return (components_.find(idx) != components_.end());
}


void Entity::ReplaceWith(const int idx, std::unique_ptr<IComponent> replacement)
{
	IComponent* prev_component = GetComponent(idx);
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
			replacement->SetOwner(this);
			components_[idx] = std::move(replacement);
		}
	}
}

void Entity::DestroyAllComponent()
{
	components_.clear();
}

