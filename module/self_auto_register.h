#pragma once

#include <type_traits>
#include "module_interface.h"
#include "module_manager.h"

namespace terra
{
    template <typename T, typename = std::enable_if_t<std::is_base_of<IModule, T>::value>>
    class SelfAutoRegister
    {
    public:
        static bool AutoRegister(const char* module_name)
        {
			//ModuleManager::GetInstance().RegisterModule(module_name, ...)
            T::GetInstance();
            return true;
        }
    };

    /**	warning: the singleton that using
    this method should not care about initialization sequence
    */

    // the bool variable will be optimize in release mode, so it does not matter.
#define REG_INSTANCE_MOUDLE(class_name) \
    \
static bool class_name##_singleton_register = TAutoRegister<class_name>::AutoRegister(#class_name);
}