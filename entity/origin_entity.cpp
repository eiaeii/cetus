#include "origin_entity.h"
#include "component.h"

using namespace terra;

OriginEntity* OriginEntity::default_instance_ = nullptr;

OriginEntity* OriginEntity::GetInstance()
{
	if (!default_instance_)
	{
		default_instance_ = new OriginEntity();
	}
	return default_instance_;
}

void OriginEntity::Destroy()
{
	delete default_instance_;
}


OriginEntity::OriginEntity(){}

OriginEntity::~OriginEntity()
{
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
		if (kv.second->CanUpdate())
		{
			(kv.second)->PreUpdate();
		}
	}
}

void OriginEntity::Update()
{
	for (auto&& kv : components_)
	{
		if (kv.second->CanUpdate())
		{
			(kv.second)->Update();
		}
	}
}

void OriginEntity::PostUpdate()
{
	for (auto&& kv : components_)
	{
		if (kv.second->CanUpdate())
		{
			(kv.second)->PostUpdate();
		}
	}
}

void OriginEntity::Exit()
{
	for (auto&& kv : components_)
	{
		(kv.second)->Exit();
	}
}

