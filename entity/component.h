#pragma once
#include "ecs_util.h"
#include "updatable_interface.h"

namespace terra
{
	class Entity;
	class IComponent : IUpdatable
	{
	protected:
		Entity* ent_{ nullptr };

	public:
		IComponent();
		virtual ~IComponent();

		void SetOwner(Entity* ent) { ent_ = ent; }
		Entity* Owner() { return ent_; }

		virtual bool Awake() override { return true; }
		virtual bool Init() override { return true; }
		virtual void PreUpdate() override {}
		virtual void Update() override {}
		virtual void PostUpdate() override {}
		virtual void Exit() override {}
		virtual void Destroy() override {}

		virtual bool CanUpdate() override { return false; }
		virtual void SetUpdatable(bool update) override {}

		template <typename C>
		C* As();
		template <typename C>
		C const* As() const;

		template<class C>
		C* Sibling();

	private:
		IComponent* SiblingComponent(const int idx);
	};

	template <typename C>
	C* IComponent::As()
	{
		TERRA_ASSERT_IS_COMPONENT(C);
		return static_cast<C*>(this);
	}

	template <typename C>
	C const* IComponent::As() const
	{
		TERRA_ASSERT_IS_COMPONENT(C);
		return static_cast<C*>(this);
	}
	template<class C>
	C* IComponent::Sibling()
	{
		TERRA_ASSERT_IS_COMPONENT(C);
		IComponent* component = SiblingComponent(ComponentIdPool::index<C>());
		return component ? component->As<C>() : nullptr;
	}

}