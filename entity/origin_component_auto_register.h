#pragma once

#include <type_traits>
#include "origin_module_interface.h"
#include "origin_entity.h"

namespace terra
{
    template <typename C>
    class OriginComponentAutoRegister
    {
    public:
        static inline bool AutoRegister()
        {
			TERRA_ASSERT_IS_ORIGIN_COMPONENT(C);
			OriginEntity::Get()->Add<C>();
            return true;
        }
    };

    /**	warning: the singleton that using
    this method should not care about initialization sequence
    */

    // the bool variable will be optimize in release mode, so it does not matter.
#define REG_ORIGIN_COMPONENT(class_name) \
    \
static bool class_name##_singleton_register = OriginComponentAutoRegister<class_name>::AutoRegister()
}