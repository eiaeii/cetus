#include "entity/component.h"
#include "entity.h"

using namespace terra;

IComponent::IComponent()
{
}

IComponent::~IComponent()
{
}

IComponent* IComponent::SiblingComponent(const int idx)
{
	return ent_ ? ent_->GetComponent(idx) : nullptr;
}