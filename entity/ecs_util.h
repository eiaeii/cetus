#pragma once

namespace terra
{

#define TERRA_ASSERT_IS_ENTITY(E)                                                                      \
    static_assert((std::is_base_of<Entity, E>::value), \
                  "Class type must be Entity");

#define TERRA_ASSERT_IS_COMPONENT(C)                                                                      \
    static_assert((std::is_base_of<IComponent, C>::value && !std::is_same<IComponent, C>::value), \
                  "Class type must be derived from IComponent");

	struct ComponentIdPool 
	{
		template <typename C>
		static int index()
		{
			static int idx = count()++;
			return idx;
		}
		static int& count()
		{
			static int counter = 0;
			return counter;
		}
	};
}