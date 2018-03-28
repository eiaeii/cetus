#pragma once

#include "module_interface.h"
#include "global_macro.h"
#include <map>
namespace terra
{
	class ModuleManager : public IModule
	{
		MAKE_INSTANCE(ModuleManager)
	private:
		std::map<std::string, IModule*> modules_;
	public:
		void RegisterModule(const char* module_name, IModule* md);
		template<typename T>
		T* FindModule();

		virtual void Awake() override;
		virtual void Init() override;
		virtual void PreUpdate() override;
		virtual void Update() override;
		virtual void PostUpdate() override;
		virtual void Exit() override;
		virtual void Destroy() override;
	};
	template<typename T>
	T* ModuleManager::FindModule()
	{
		auto it = modules_.find(T::ClassName());
		return it != modules_.end() ? static_cast<T*>(it->second) : nullptr;
	}
}