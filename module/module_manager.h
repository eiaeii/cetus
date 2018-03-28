#pragma once

#include "module_interface.h"
#include "global_macro.h"

namespace terra
{
	class ModuleManager
	{
		MAKE_INSTANCE(ModuleManager)
	public:
		void RegisterModule(const char* module_name, IModule* md);
		void UnregisterModule(const char* module_name, IModule* md);
		// FindModule
	};

}