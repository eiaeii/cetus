#pragma once

#include "core.h"
#include "update_interface.h"
#include "entity.h"

namespace terra
{
	class OriginComponent;
    class OriginEntity : public Entity, public IUpdatable
    {
    private:
		bool updatable_{ true };
        static OriginEntity* default_instance_;
	public:
		static OriginEntity* GetInstance();
		static void Destroy();

    public:
        OriginEntity();
        virtual ~OriginEntity();

		virtual bool Awake() override;
		virtual bool Init() override;
		virtual void PreUpdate() override;
		virtual void Update() override;
		virtual void PostUpdate() override;
		virtual void Exit() override;

		virtual bool CanUpdate() override { return updatable_; }
		virtual void SetUpdatable(bool update) override { updatable_ = update; }
    };
}