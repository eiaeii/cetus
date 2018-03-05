#pragma once

namespace terra
{
	template<typename T>
	struct is_enum_class
	{
		enum { value = std::is_enum<T>::value && !std::is_convertible<T, int>::value };
	};
}