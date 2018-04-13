#pragma once

#include "core.h"
#include "update_interface.h"
#include "type_traits_ex.h"

namespace terra
{
	class IComponent;
	class Entity
	{
	protected:
		std::unordered_map<int, std::unique_ptr<IComponent>> components_;
	public:
		Entity();
		virtual ~Entity();

		template <typename C, typename = std::enable_if<is_unique_ptr<C>::value>::type>
		auto Add(std::unique_ptr<C> c)->Entity&;

		template <typename C, typename... Args>
		auto Add(Args&&... args)->Entity&;

		template <typename Arg>
		auto Remove()->Entity&;
		template <typename Arg0, typename... Args>
		auto Remove() -> typename std::enable_if<sizeof...(Args) != 0, Entity&>::type;

		template <typename C, typename = std::enable_if<is_unique_ptr<C>::value>::type>
		auto Replace(std::unique_ptr<C> c)->Entity&;

		template <typename C, typename... Args>
		auto Replace(Args&&... args)->Entity&;

		template <typename C>
		auto Get() const->C*;
		template <typename... Args>
		auto Get() const -> typename std::enable_if<(sizeof...(Args) != 1), std::tuple<Args*...>>::type;

		template <typename Arg>
		bool Has() const;
		template <typename Arg0, typename... Args>
		auto Has() const -> typename std::enable_if<sizeof...(Args) != 0, bool>::type;

		IComponent* GetComponent(const int idx) const;

	private:
		Entity& AddComponent(const int idx, std::unique_ptr<IComponent> component);
		Entity& RemoveComponent(const int idx);
		Entity& ReplaceComponent(const int idx, std::unique_ptr<IComponent> component);
		bool HasComponent(const int idx) const;
		void ReplaceWith(const int idx, std::unique_ptr<IComponent> component);

		void DestroyAllComponent();
	};

	template<typename C, typename>
	auto Entity::Add(std::unique_ptr<C> c) -> Entity&
	{
		return AddComponent(ComponentIdPool::index<C>(), std::move(c));
	}

	template <typename C, typename... Args>
	auto Entity::Add(Args&&... args) -> Entity&
	{
		return AddComponent(ComponentIdPool::index<C>(),
			std::make_unique<IComponent>(std::forward<Args>(args)...));
	}

	template <typename Arg>
	auto Entity::Remove() -> Entity&
	{
		return RemoveComponent(ComponentIdPool::index<Arg>());
	}
	template <typename Arg0, typename... Args>
	auto Entity::Remove() -> typename std::enable_if<sizeof...(Args) != 0, Entity&>::type
	{
		return RemoveComponent(ComponentIdPool::index<Arg0>()), Remove<Args...>();
	}

	template<typename C, typename>
	inline auto Entity::Replace(std::unique_ptr<C> c) -> Entity &
	{
		return ReplaceComponent(ComponentIdPool::index<C>(), std::move(c));
	}

	template <typename C, typename... Args>
	auto Entity::Replace(Args&&... args) -> Entity&
	{
		return ReplaceComponent(ComponentIdPool::index<C>(),
			std::make_unique<IComponent>(std::forward<Args>(args)...));
	}

	template <typename C>
	auto Entity::Get() const -> C*
	{
		return static_cast<C*>(GetComponent(ComponentIdPool::index<C>()));
	}

	template <typename... Args>
	auto Entity::Get() const ->
		typename std::enable_if<(sizeof...(Args) != 1), std::tuple<Args*...>>::type
	{
		using ValueType = std::tuple<Args*...>;
		if (Has<Args...>()) {
			return ValueType(Get<Args>()...);
		}
		return ValueType();
	}

	template <typename Arg>
	bool Entity::Has() const
	{
		return HasComponent(ComponentIdPool::index<Arg>());
	}

	template <typename Arg0, typename... Args>
	auto Entity::Has() const -> typename std::enable_if<sizeof...(Args) != 0, bool>::type
	{
		return HasComponent(ComponentIdPool::index<Arg0>()) && Has<Args...>();
	}

}