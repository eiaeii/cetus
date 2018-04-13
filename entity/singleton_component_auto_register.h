#pragma once

#include <type_traits>
#include "origin_entity.h"

namespace terra
{
    template <typename C>
    class SingletonComponentAutoRegister
    {
    public:
		template<typename E>
        static inline bool AutoRegister(E* ent)
        {
			TERRA_ASSERT_IS_ENTITY(E);
			TERRA_ASSERT_IS_COMPONENT(C);
			ent->Add<C>();
            return true;
        }
    };

    /**	warning: the singleton that using
    this method should not care about initialization sequence
    */

    // the bool variable will be optimize in release mode, so it does not matter.
#define REG_SINGLETON_COMPONENT(class_name, entity_pointer) \
    \
static bool class_name##_singleton_register = SingletonComponentAutoRegister<class_name>::AutoRegister(entity_pointer)
}