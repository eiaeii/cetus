#pragma once

#include "core.h"
#include "updatable_interface.h"
#include "type_traits_ex.h"
#include "ecs_util.h"

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

		template <typename C, typename... Args>
		auto Add(Args&&... args)->Entity&;

		template <typename C>
		auto Remove()->Entity&;
		template <typename C0, typename... Cs>
		auto Remove() -> typename std::enable_if<sizeof...(Cs) != 0, Entity&>::type;


		template <typename C, typename... Args>
		auto Replace(Args&&... args)->Entity&;

		template <typename C>
		auto Get() const->C*;
		template <typename... Cs>
		auto Get() const -> typename std::enable_if<(sizeof...(Cs) != 1), std::tuple<Cs*...>>::type;

		template <typename C>
		bool Has() const;
		template <typename C0, typename... Cs>
		auto Has() const -> typename std::enable_if<sizeof...(Cs) != 0, bool>::type;

		IComponent* GetComponent(const int idx) const;

		void DestroyAllComponent();
	private:
		Entity& AddComponent(const int idx, std::unique_ptr<IComponent> component);
		Entity& RemoveComponent(const int idx);
		Entity& ReplaceComponent(const int idx, std::unique_ptr<IComponent> component);
		bool HasComponent(const int idx) const;
		void ReplaceWith(const int idx, std::unique_ptr<IComponent> component);

	};

	template <typename C, typename... Args>
	auto Entity::Add(Args&&... args) -> Entity&
	{
		return AddComponent(ComponentIdPool::index<C>(),
			std::make_unique<C>(std::forward<Args>(args)...));
	}

	template <typename C>
	auto Entity::Remove() -> Entity&
	{
		return RemoveComponent(ComponentIdPool::index<C>());
	}
	template <typename C0, typename... Cs>
	auto Entity::Remove() -> typename std::enable_if<sizeof...(Cs) != 0, Entity&>::type
	{
		return RemoveComponent(ComponentIdPool::index<C0>()), Remove<Cs...>();
	}

	template <typename C, typename... Args>
	auto Entity::Replace(Args&&... args) -> Entity&
	{
		return ReplaceComponent(ComponentIdPool::index<C>(),
			std::make_unique<C>(std::forward<Args>(args)...));
	}

	template <typename C>
	auto Entity::Get() const -> C*
	{
		return static_cast<C*>(GetComponent(ComponentIdPool::index<C>()));
	}

	template <typename... Cs>
	auto Entity::Get() const ->
		typename std::enable_if<(sizeof...(Cs) != 1), std::tuple<Cs*...>>::type
	{
		using ValueType = std::tuple<Cs*...>;
		if (Has<Cs...>()) {
			return ValueType(Get<Cs>()...);
		}
		return ValueType();
	}

	template <typename C>
	bool Entity::Has() const
	{
		return HasComponent(ComponentIdPool::index<C>());
	}

	template <typename C0, typename... Cs>
	auto Entity::Has() const -> typename std::enable_if<sizeof...(Cs) != 0, bool>::type
	{
		return HasComponent(ComponentIdPool::index<C0>()) && Has<Cs...>();
	}

}