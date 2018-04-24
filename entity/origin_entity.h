#pragma once

#include "core.h"
#include "updatable_interface.h"
#include "entity.h"

namespace terra
{
    class OriginEntity : public Entity, public IUpdatable
    {
    private:
		const FGuid guid_;
		bool updatable_{ true };
        static OriginEntity* default_instance_;
	public:
		static OriginEntity* GetInstance();
		static void DestroyInstance();

	private:
		OriginEntity();
		virtual ~OriginEntity();
    public:
		const FGuid& GetGuid() const { return guid_; }

		virtual bool Awake() override;
		virtual bool Init() override;
		virtual void PreUpdate() override;
		virtual void Update() override;
		virtual void PostUpdate() override;
		virtual void Exit() override;
		virtual void Destroy() override;

		virtual bool CanUpdate() override { return updatable_; }
		virtual void SetUpdatable(bool update) override { updatable_ = update; }
    };

#define GETCOMPONENT(component_name) OriginEntity::GetInstance()->Get<component_name>()
}