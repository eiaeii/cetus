#pragma once

namespace terra
{
#define TERRA_ASSERT_IS_ORIGIN_COMPONENT(C)                                                                      \
    static_assert((std::is_base_of<OriginComponent, C>::value && !std::is_same<OriginComponent, C>::value), \
                  "Class type must be derived from OriginComponent");

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