#pragma once
#include "origin_util.h"
#include "origin_entity.h"

namespace terra
{
    class OriginComponent : IModule
    {
	public:
        OriginComponent() = default;
        virtual ~OriginComponent() = default;

        template <typename C>
        C* As();
        template <typename C>
        C const* As() const;

        virtual bool Awake() override { return true; }
        virtual bool Init() override { return true; }
        virtual void PreUpdate() override {}
        virtual void Update() override {}
        virtual void PostUpdate() override {}
        virtual void Exit() override {}
    };

    template <typename C>
    C* OriginComponent::As()
    {
        TERRA_ASSERT_IS_ORIGIN_COMPONENT(C);
        return static_cast<C*>(this);
    }

    template <typename C>
    C const* OriginComponent::As() const
    {
        TERRA_ASSERT_IS_ORIGIN_COMPONENT(C);
        return static_cast<C*>(this);
    }
}