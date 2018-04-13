#pragma once
#include "ecs_util.h"
#include "component.h"

namespace terra
{
    class SingletonComponent : IComponent
	{
	public:
		SingletonComponent() = default;
        virtual ~SingletonComponent() = default;

		virtual bool CanUpdate() override { return true; }
    };
}