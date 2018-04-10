#include "module_manager.h"
#include "gsl_assert.h"
using namespace terra;

void ModuleManager::RegisterModule(const char* module_name, IModule* md)
{
	Expects(modules_.find(module_name) == modules_.end());
	modules_[module_name] = md;
}

bool terra::ModuleManager::Running() const
{
	return running_;
}

void ModuleManager::Stop(bool run)
{
	running_ = run;
}

bool ModuleManager::Awake()
{
	for (auto&& kv : modules_)
	{
		if(!(kv.second)->Awake())
		{
			return false;
		}
	}
	return true;
}

bool ModuleManager::Init()
{
	for (auto&& kv : modules_)
	{
		if (!(kv.second)->Init())
		{
			return false;
		}
	}
	return true;
}

void ModuleManager::PreUpdate()
{
	for (auto&& kv : modules_)
	{
		(kv.second)->PreUpdate();
	}
}

void ModuleManager::Update()
{
	for (auto&& kv : modules_)
	{
		(kv.second)->Update();
	}
}

void ModuleManager::PostUpdate()
{
	for (auto&& kv : modules_)
	{
		(kv.second)->PostUpdate();
	}
}

void ModuleManager::Exit()
{
	for (auto&& kv : modules_)
	{
		(kv.second)->Exit();
	}
}

void ModuleManager::Destroy()
{
	for (auto&& kv : modules_)
	{
		(kv.second)->Destroy();
	}
}
