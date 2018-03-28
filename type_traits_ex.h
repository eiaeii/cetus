#pragma once

#include <type_traits>
#include <tuple>

namespace terra
{
	namespace details
	{
		template<class...> struct conjunction : std::true_type { };
		template<class B1> struct conjunction<B1> : B1 { };
		template<class B1, class... Bn>
		struct conjunction<B1, Bn...>
			: std::conditional_t<bool(B1::value), conjunction<Bn...>, B1> {};

		template<typename T, typename U>
		struct is_equivalent : std::integral_constant<bool, std::is_same<T, U>::value || std::is_convertible<T, U>::value> {};

		template<bool same_size, typename T, typename... Us>
		struct tuple_match_impl;

		template<typename... Ts, typename... Us>
		struct tuple_match_impl<true, std::tuple<Ts...>, Us...> : conjunction<is_equivalent<Ts, Us>...> {};

		template<typename... Ts, typename... Us>
		struct tuple_match_impl<false, std::tuple<Ts...>, Us...> : std::false_type {};

	}
	template<typename... Ts> struct make_void { typedef void type; };
	template<typename... Ts> using void_t = typename make_void<Ts...>::type;

	template<typename T, typename... Us>
	struct tuple_match : std::false_type {};
	template<typename... Ts, typename... Us>
	struct tuple_match<std::tuple<Ts...>, Us...> : details::tuple_match_impl<(sizeof...(Ts) == sizeof...(Us)), std::tuple<Ts...>, Us...>
	{
	};

	template<typename T>
	struct is_enum_class
	{
		enum { value = std::is_enum<T>::value && !std::is_convertible<T, int>::value };
	};

	template <typename T>
	using is_vector = std::is_same<T, std::vector< typename T::value_type, typename T::allocator_type > >;
}